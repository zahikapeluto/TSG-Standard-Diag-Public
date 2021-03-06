/**
* MDMI sample Code for Subscribe/Unsubscribe v2.0
*/

#include "stdlib.h"
#include "MDMI.h"

// How to use MdmiSubscribe
static void SubscribeSample(MdmiSession session, MdmiObjectName name)
{
    MdmiError result = MDMI_NO_ERROR;
    result = MdmiSubscribe(session, &name);
    DebugWriteLine("MdmiSubscribe result: %u", result);
}

// How to use MdmiUnsubscribe
static void UnsubscribeSample(MdmiSession session, MdmiObjectName name)
{
    MdmiError result = MDMI_NO_ERROR;
    result = MdmiUnsubscribe(session, &name);
    DebugWriteLine("MdmiUnsubscribe result: %u", result);
}

// How to use MdmiSetEventCallback
static void SetEventCallbackSample(MdmiSession session)
{
    MdmiError result = MDMI_NO_ERROR;

    result = MdmiSetEventCallback(session, &EventCallback, NULL);
    DebugWriteLine("MdmiInvoke result: %u", result);
}

static void EventCallback(MdmiSession session, const MdmiObjectName* eventName, const MdmiValue* value, void* state)
{
    DebugWriteLine("EventCallback session: %u, eventName: %u, value: %x, state: %x", session, eventName->length, value, state);
	free(value);
}

static void DebugWriteObjectName(const MdmiObjectName* name)
{
    if (NULL == name)
    {
        return;
    }

    for (MdmiTypeUInt32 i = 0; i < name->length; ++i){
        DebugWriteLine("name [%u] %u", i, name->ids[i]);
    }
}

static void DebugWriteValue(const MdmiValue* value)
{
    if (NULL == value)
    {
        return;
    }

    for (MdmiTypeUInt32 i = 0; i < value->length; ++i){
        DebugWriteLine("value [%u] %u", i, value->data[i]);
    }
}


extern void TestMdmiApi()
{
    // Create a MDMI session
    MdmiError result = MDMI_NO_ERROR;
    MdmiSession session = 0;

    result = MdmiCreateSession(NULL, &session);
    DebugWriteLine("MdmiCreateSession result: %u", result);

    if (result != MDMI_NO_ERROR) {
        DebugWriteLine("Failed to initialize MDMI session");
        return;
    }

    // After session created

    // Set call back function
    SetEventCallbackSample(session);
    
    // subscribe all interested OIDs
    // In this sample, it will subcribe two OIDs as how-to example.
    // 1.2.7.2.1   ServingCellMeasurement
    MdmiObjectName name1 = { 0 };
    name1.length = 5;
	MdmiTypeUInt32[] oid1 = {1, 2, 7, 2, 1};
    name1.ids = oid1;

    // 1.2.7.2.2   NeighborCellMeasurements
    MdmiObjectName name2 = { 0 };
    name2.length = 5;
	MdmiTypeUInt32[] oid2 = {1, 2, 7, 2, 2};
    name2.ids = oid2;

    // Please refer to SubscribeSample() definition above.
    SubscribeSample(session， name1);
    SubscribeSample(session, name2);


    // At this point, app will start receiving subscribed logs


    // Unsubscribe all subscribed OIDs before closing session
    UnsubscribeSample(session, name1);
    UnsubscribeSample(session, name2);

    // Close MDMI session
    result = MdmiCloseSession(session);
    DebugWriteLine("MdmiCloseSession result: %u", result);
}


