<?php

namespace App\Http\Controllers\Dashboard;

use App\Http\Controllers\Controller;
use Illuminate\Http\Request;
use App\Services\OPCUAClient;

class OPCUAController extends Controller
{
    private $client;

    public function __construct(OPCUAClient $client)
    {
        $this->client = $client;
    }

    public function show(Request $request, string $pump)
    {
        $pumpNodeID = "ns=2;s=" . $pump;
        $result = $this->client->getDataNodes($pumpNodeID);

        $pumpData = [];
        if ($result["status"]) {
            foreach ($result["value"] as $nodeElement) {
                $data = [];
                $data["name"] =  $nodeElement->DisplayName;

                $nodeId = $this->client->getNodeId($nodeElement->NodeId);
                $nodeValue = $this->client->getNodeValue($nodeId);
                if ($nodeValue["status"]) {
                    $data["value"] = $nodeValue["value"];
                } else {
                    $data["value"] = "";
                }
                $pumpData[] = $data;
            }
        }

        $pumpMethods = [];
        $result = $this->client->getMethodsNodes($pumpNodeID);
        if ($result["status"]) {
            foreach ($result["value"] as $nodeMethod) {
                $data = [];
                $data["name"] =  $nodeMethod->DisplayName;
                $data["methodId"] = $this->client->getNodeId($nodeMethod->NodeId);
                $pumpMethods[] = $data;
            }
        }
        return view('dashboard.opcua.show', compact('pump', 'pumpData', 'pumpMethods'));
    }

    public function callMethod(Request $request, string $pump, string $method)
    {
        $objectId = "ns=2;s=" . $pump;
        $this->client->callMethodWithoutParameters($objectId, $method);
        session()->flash('success', 'Method Called Successfully');
        return redirect()->back();
    }

    public function index(Request $request)
    {
        //to-do:
        //need to get pumps from opc ua server
        return view('dashboard.opcua.index');
    }
}
