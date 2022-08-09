#include <signal.h>
#include <stdlib.h>
#include "open62541.h"

/* predefined identifier for later use */
UA_Boolean running = true;
UA_NodeId pumpTypeId;
UA_NodeId pump1MotorRPMNodeId;
UA_NodeId pump2MotorRPMNodeId;

static void stopHandler(int sign)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

static void
beforeReadPump1MotorRPM(UA_Server *server,
                        const UA_NodeId *sessionId, void *sessionContext,
                        const UA_NodeId *nodeid, void *nodeContext,
                        const UA_NumericRange *range, const UA_DataValue *data)
{

    double temp = 1.0 * (rand() % 50) + 1.0;
    UA_Variant value;
    UA_Variant_setScalar(&value, &temp, &UA_TYPES[UA_TYPES_DOUBLE]);
    UA_Server_writeValue(server, pump1MotorRPMNodeId, value);
} // beforeReadPump1MotorRPM

static UA_StatusCode
startPumpMethodCallback(UA_Server *server,
                        const UA_NodeId *sessionId, void *sessionHandle,
                        const UA_NodeId *methodId, void *methodContext,
                        const UA_NodeId *objectId, void *objectContext,
                        size_t inputSize, const UA_Variant *input,
                        size_t outputSize, UA_Variant *output)
{
    /* Find the NodeId of the isOn child variable */
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = UA_QUALIFIEDNAME(2, "isOn");

    UA_BrowsePath bp;
    UA_BrowsePath_init(&bp);
    bp.startingNode = *objectId;
    bp.relativePath.elementsSize = 1;
    bp.relativePath.elements = &rpe;

    UA_BrowsePathResult bpr =
        UA_Server_translateBrowsePathToNodeIds(server, &bp);
    if (bpr.statusCode != UA_STATUSCODE_GOOD ||
        bpr.targetsSize < 1)
        return bpr.statusCode;

    /* Set the isOn value */
    UA_Boolean isOn = true;
    UA_Variant value;
    UA_Variant_setScalar(&value, &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, bpr.targets[0].targetId.nodeId, value);
    UA_BrowsePathResult_clear(&bpr);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "start pump from method");
    return UA_STATUSCODE_GOOD;
} // startPumpMethodCallback

static UA_StatusCode
stopPumpMethodCallback(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionHandle,
                       const UA_NodeId *methodId, void *methodContext,
                       const UA_NodeId *objectId, void *objectContext,
                       size_t inputSize, const UA_Variant *input,
                       size_t outputSize, UA_Variant *output)
{
    /* Find the NodeId of the isOn child variable */
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = UA_QUALIFIEDNAME(2, "isOn");

    UA_BrowsePath bp;
    UA_BrowsePath_init(&bp);
    bp.startingNode = *objectId;
    bp.relativePath.elementsSize = 1;
    bp.relativePath.elements = &rpe;

    UA_BrowsePathResult bpr =
        UA_Server_translateBrowsePathToNodeIds(server, &bp);
    if (bpr.statusCode != UA_STATUSCODE_GOOD ||
        bpr.targetsSize < 1)
        return bpr.statusCode;

    /* Set the isOn value */
    UA_Boolean isOn = false;
    UA_Variant value;
    UA_Variant_setScalar(&value, &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_Server_writeValue(server, bpr.targets[0].targetId.nodeId, value);
    UA_BrowsePathResult_clear(&bpr);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "stop pump from method");
    return UA_STATUSCODE_GOOD;
} // stopPumpMethodCallback

static void
definePumpType(UA_Server *server)
{
    UA_ObjectTypeAttributes dtAttr = UA_ObjectTypeAttributes_default;
    dtAttr.displayName = UA_LOCALIZEDTEXT("en-US", "PumpType");
    UA_Server_addObjectTypeNode(server, UA_NODEID_STRING(2, "PumpType"),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(2, "PumpType"), dtAttr,
                                NULL, &pumpTypeId);

} // definePumpType

static void
definePump(UA_Server *server, char *pumpName, char *pumpManufacturer,
           char *pumpManufacturerNodeId, char *pumpModel, char *pumpModelNodeId,
           bool pumpIsOn, char *pumpIsOnNodeId, double pumpMotorRPM,
           char *pumpMotorRPMNodeName, UA_NodeId *pumpMotorRPMNodeId,
           char *startPumpNodeId, char *stopPumpNodeId)
{
    UA_NodeId pumpId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", pumpName);
    UA_Server_addObjectNode(server, UA_NODEID_STRING(2, pumpName),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(2, pumpName),
                            pumpTypeId, /* this refers to the object type identifier */
                            oAttr, NULL, &pumpId);

    UA_VariableAttributes mnAttr = UA_VariableAttributes_default;
    mnAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    UA_String manufacturerName = UA_STRING(pumpManufacturer);
    UA_Variant_setScalar(&mnAttr.value, &manufacturerName, &UA_TYPES[UA_TYPES_STRING]);
    mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Manufacturer Name");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, pumpManufacturerNodeId), pumpId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "ManufacturerName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                              mnAttr, NULL, NULL);

    UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
    modelAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    UA_String modelName = UA_STRING(pumpModel);
    UA_Variant_setScalar(&modelAttr.value, &modelName, &UA_TYPES[UA_TYPES_STRING]);
    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Model Name");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, pumpModelNodeId), pumpId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "ModelName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), modelAttr, NULL, NULL);

    UA_VariableAttributes isOnAttr = UA_VariableAttributes_default;
    isOnAttr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    UA_Boolean isOn = pumpIsOn;
    UA_Variant_setScalar(&isOnAttr.value, &isOn, &UA_TYPES[UA_TYPES_BOOLEAN]);
    isOnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Is On");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, pumpIsOnNodeId), pumpId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "isOn"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), isOnAttr, NULL, NULL);

    UA_VariableAttributes rpmAttr = UA_VariableAttributes_default;
    rpmAttr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    UA_Double rpm = pumpMotorRPM;
    UA_Variant_setScalar(&rpmAttr.value, &rpm, &UA_TYPES[UA_TYPES_DOUBLE]);
    rpmAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Motor RPM");
    UA_Server_addVariableNode(server, UA_NODEID_STRING(2, pumpMotorRPMNodeName), pumpId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(2, "MotorRPM"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), rpmAttr, NULL, pumpMotorRPMNodeId);

    UA_MethodAttributes startPumpAttr = UA_MethodAttributes_default;
    startPumpAttr.description = UA_LOCALIZEDTEXT("en-US", "Start Pump");
    startPumpAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Start Pump");
    startPumpAttr.executable = true;
    startPumpAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(2, startPumpNodeId), pumpId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(2, "Start Pump"),
                            startPumpAttr, &startPumpMethodCallback,
                            0, NULL, 0, NULL, NULL, NULL);

    UA_MethodAttributes stopPumpAttr = UA_MethodAttributes_default;
    stopPumpAttr.description = UA_LOCALIZEDTEXT("en-US", "Stop Pump");
    stopPumpAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Stop Pump");
    stopPumpAttr.executable = true;
    stopPumpAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(2, stopPumpNodeId), pumpId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(2, "Stop Pump"),
                            stopPumpAttr, &stopPumpMethodCallback,
                            0, NULL, 0, NULL, NULL, NULL);

} // definePump

int main(int argc, char **argv)
{
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_UInt16 portNumber = 7077;
    UA_ServerConfig_setMinimal(UA_Server_getConfig(server), portNumber, 0);

    // add a new namespace to the server
    UA_UInt16 ns[2];
    ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
    ns[1] = UA_Server_addNamespace(server, "http://test.org/nodeset/");

    definePumpType(server);

    definePump(server, "pump1", "manufacturer 1", "pump1-manufacturer",
               "model 100", "pump1-model",
               true, "pump1-isOn", 50.0, "pump1-MotorRPM", &pump1MotorRPMNodeId,
               "startPump1", "stopPump1");
    UA_ValueCallback callback;
    callback.onRead = beforeReadPump1MotorRPM;
    callback.onWrite = NULL;
    UA_Server_setVariableNode_valueCallback(server, pump1MotorRPMNodeId, callback);

    definePump(server, "pump2", "manufacturer 2", "pump2-manufacturer",
               "model 200", "pump2-model",
               false, "pump2-isOn", 60.0, "pump2-MotorRPM", &pump2MotorRPMNodeId,
               "startPump2", "stopPump2");

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "starting server ...");
    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "server was shut down ...");
    UA_Server_delete(server);
    return (int)retval;
}