#include "Vive.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>

//#include <openvr.h>

using namespace OSG;
using namespace std;

/**
	openvr gives you events (as struct) to read out. 4 things are contained:
		EVREventType eventType - The type of the event. Determines what part of the data enum is populated for this event. See the Event Type section below.
		TrackedDeviceIndex_t trackedDeviceIndex - The tracked device index of the event. For events that aren't connected to a tracked device this is k_unTrackedDeviceIndexInvalid.
		VREvent_Data_t data - More information about the event. This is a union of several structs. See the event type enum for information about which union member to look at for each event.
		float eventAgeSeconds - The age of the event in seconds.
	event types are:
		VREvent_None - The event is invalid.
		VREvent_TrackedDeviceActivated - A tracked device was plugged in or otherwise detected by the system. There is no data, but the trackedDeviceIndex will be the index of the new device.
		VREvent_TrackedDeviceDeactivated - A tracked device was unplugged or the system is no longer able to contact it in some other way. Data is not used for this event.
		VREvent_TrackedDeviceUpdated - One or more of the properties of a tracked device have changed. Data is not used for this event.
		VREvent_ButtonPress - The user has pressed a button on a controller. The controller struct in data identifies the button.
		VREvent_ButtonUnpress - The user has stopped pressing a button on a controller. The controller struct in data identifies the button.
		VREvent_ButtonTouch - The user has touched a button on a controller. The controller struct in data identifies the button. This event will always happen before Press.
		VREvent_ButtonUntouch - The user has stopped touching a button on a controller. The controller struct in data identifies the button. The event will always happen after Unpress.
		VREvent_MouseMove - This is an internal simulated mouse move event. The mouse struct in data identifies details. This event is only generated by simulated mouse input, not by system mouse events.
		VREvent_MouseButtonDown - This is an internal simulated mouse button down event. The mouse struct in data identifies details. This event is only generated by simulated mouse input, not by system mouse events.
		VREvent_MouseButtonUp - This is an internal simulated mouse button up event. The mouse struct in data identifies details. This event is only generated by simulated mouse input, not by system mouse events.
		VREvent_InputFocusCaptured - Another application has called CaptureInputFocus. The process struct in data identifies the process. This event is not sent to the application that captured the input focus.
		VREvent_InputFocusReleased - Another application has called ReleaseInputFocus. The process struct in data identifies the process. This event is not sent to the application that released the input focus
 **/


Vive::Vive() {
	/*if (!vr::VR_IsHmdPresent()) return;

	try {
        vr::EVRInitError err = vr::VRInitError_None;
        HMD = vr::VR_Init( &err, vr::VRApplication_Scene );
        ready = true;
        cout << "---- Vive ready! ----" << endl;
	} catch (exception e) {
        ready = false;
        cout << "Vive exception: " << e.what() << endl;
    }*/
}

Vive::~Vive() {}

void Vive::update() {
    if (!ready) return;
    processEvent();
    updatePoses();
    processController();
}

void Vive::processEvent(){
	/*vr::VREvent_t event;
	while( HMD->PollNextEvent( &event, sizeof( event ) ) ) {
		switch(event.eventType) {
		case vr::VREvent_TrackedDeviceActivated:
			std::cout << "VREvent_TrackedDeviceActivated: id " << event.trackedDeviceIndex << std::endl;
			break;
		case vr::VREvent_TrackedDeviceDeactivated:
            std::cout << "VREvent_TrackedDeviceDeactivated: id " << event.trackedDeviceIndex << std::endl;
			break;
		case vr::VREvent_ButtonPress:
            HMD->ResetSeatedZeroPose();
            std::cout << "VREvent_ButtonPress: id " << event.trackedDeviceIndex << std::endl;
			break;
		case vr::VREvent_TrackedDeviceUpdated:
            std::cout << "VREvent_TrackedDeviceUpdated for device" << event.trackedDeviceIndex << std::endl;
			break;
		}
	}*/
}

/*Matrix4d Vive::convMat( const vr::HmdMatrix34_t &m ) {
    return Matrix4d(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		0, 0, 0, 1 );
}*/

void Vive::processController(){
		// Process SteamVR controller state
	/*for( vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++ ) {
		if(HMD->GetTrackedDeviceClass( i ) == vr::TrackedDeviceClass_Controller){
			vr::VRControllerState_t state;
			if( HMD->GetControllerState( i, &state ) )
			{
				//state.unPacketNum - save and compare in next call...if same, no changes to kontroller state, so ignore this...
				//state.ulButtonPressed | state.ulButtonTouched - returns sum of button masks pressed

				// 2 - Menu, both are set
				// 4 - side (squeeze), both state's set
				// 429... - Touchpad
				// 8589... - trigger

				std::cout << "[" << state.rAxis[0].x << " | " << state.rAxis[0].y << "]  "; // joystick x/y axen
				std::cout << "[" << state.rAxis[1].x << " | " << state.rAxis[1].y << "]  "; //trigger...bis ca .8 druck empfindlich, bei 1 trigger richtig gepresst
				std::cout << "[" << state.rAxis[2].x << " | " << state.rAxis[2].y << "]  ";
				std::cout << "[" << state.rAxis[3].x << " | " << state.rAxis[3].y << "]  ";
				std::cout << "[" << state.rAxis[4].x << " | " << state.rAxis[4].y << "]" << std::endl;

			}
		}

	}*/

}

void Vive::updatePoses() {
	/*vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0 );

	for ( int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i ) {
		if ( poses[i].bPoseIsValid ) {
			switch (HMD->GetTrackedDeviceClass(i)) {
				case vr::TrackedDeviceClass_Controller:
					std::cout << "TrackedDeviceClass_Controller\n"<< convMat( poses[i].mDeviceToAbsoluteTracking) << std::endl;
					break;
				case vr::TrackedDeviceClass_HMD:
					std::cout << "TrackedDeviceClass_HMD\n"<< convMat( poses[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking) << std::endl;
					break;
				case vr::TrackedDeviceClass_Invalid:
					//std::cout << "TrackedDeviceClass_Invalid" << std::endl;
					break;
				case vr::TrackedDeviceClass_Other:
					//std::cout << "TrackedDeviceClass_Other" << std::endl;
					break;
				case vr::TrackedDeviceClass_TrackingReference:
					std::cout << "TrackedDeviceClass_TrackingReference\n"<< convMat( poses[i].mDeviceToAbsoluteTracking) << std::endl;
					break;
				default:
					break;
			}

		}
	}*/

}

/*
vr::IVRSystem *vr::VR_Init( vr::HmdError*peError, vr::EVRApplicationType eApplicationType )

eApplicationType must be one of:

    VRApplication_Scene - A 3D application that will be drawing an environment.
    VRApplication_Overlay - An application that only interacts with overlays or the dashboard.
    VRApplication_Background - The application will not start SteamVR. If it is not already running the call with VR_Init will fail with VRInitError_Init_NoServerForBackgroundApp.
    VRApplication_Utility - The application will start up even if no hardware is present. Only the IVRSettings and IVRApplications interfaces are guaranteed to work. This application type is appropriate for things like installers.

The call will return a vr::IVRSystem pointer that allows the game to call other OpenVR API methods. If something fails the call will return NULL and peError will be set to an error code that indicates what the problem was. peError - The error code that occurred or vr::VRInitError_None if there was no error. See vr::HmdError for possible error codes.

void vr::VR_Shutdown()

Shuts down the connection to the VR hardware and cleans up the OpenVR API. The vr::IVRSystem pointer returned by vr::VR_Init will be invalid after this call is made.
*/
