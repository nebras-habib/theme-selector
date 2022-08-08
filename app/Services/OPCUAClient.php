<?php

namespace App\Services;

use COM;
use com_exception;

class OPCUAClient
{
    private $serverEndpoint;
    private $UAClient;
    private $result;

    public function __construct()
    {
        $this->serverEndpoint = config("settings.opcua.server.endpoint");
        // Instantiate the client object
        $this->UAClient = new COM("OpcLabs.EasyOpc.UA.EasyUAClient");
        $this->result = ["status" => false, "value" => null];
    }

    public function getNodeValue($nodeId)
    {
        try {
            $data = $this->UAClient->Read($this->serverEndpoint, $nodeId);
        } catch (com_exception $exception) {
            $this->result["status"] = false;
            $this->result["value"] = $exception->getMessage();
            return $this->result;
        }

        $this->result["status"] = true;
        if (is_bool($data->Value)) {
            $this->result["value"] = $data->Value ? "True" : "False";
        } else {
            $this->result["value"] = $data->Value;
        }

        return $this->result;
    }

    public function callMethodWithoutParameters($objectId, $methodId)
    {
        try {
            // Perform the operation
            $this->UAClient->CallMethod(
                $this->serverEndpoint, //endpoint
                $objectId, //objectNode
                $methodId, //methodNode
                [], //InputArguments
                []  //InputTypeCodes
            );
        } catch (com_exception $exception) {
            $this->result["status"] = false;
            $this->result["value"] = $exception->getMessage();
            return $this->result;
        }

        $this->result["status"] = true;
        $this->result["value"] = "method called successfully";
        return $this->result;
    }

    public function getDataNodes($nodeId)
    {
        // Perform the operation
        try {
            $nodeElements = $this->UAClient->BrowseDataNodes($this->serverEndpoint, $nodeId);
        } catch (com_exception $exception) {
            $this->result["status"] = false;
            $this->result["value"] = $exception->getMessage();
            return $this->result;
        }
        $this->result["status"] = true;
        $this->result["value"] = $nodeElements;
        return $this->result;
    }

    public function getMethodsNodes($nodeId)
    {
        // Perform the operation
        try {
            $nodeElements = $this->UAClient->BrowseMethods($this->serverEndpoint, $nodeId);
        } catch (com_exception $exception) {
            $this->result["status"] = false;
            $this->result["value"] = $exception->getMessage();
            return $this->result;
        }
        $this->result["status"] = true;
        $this->result["value"] = $nodeElements;
        return $this->result;
    }

    public function getNodeId($nodeId)
    {
        // get node id 
        // for example from this form "nsu=http://test.org/nodeset/ ;ns=2;s=pump1-manufacturer"
        // to this one "ns=2;s=pump1-manufacturer"
        return substr($nodeId, strpos($nodeId, ";") + 1);
    }
}
