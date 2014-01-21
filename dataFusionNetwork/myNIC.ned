package mywsn;

import org.mixim.modules.nic.INic;
import org.mixim.modules.nic.NicController;

module myNIC like INic
{
	parameters:
		string connectionManagerName = default("");   //name of the ConnectionManager module
		
		// power consumption from boards (at 3V)
		double sleepCurrent @unit(mA) = 0.02mA; // 20 µA
		double rxCurrent @unit(mA) = 16.4 mA;
		double decodingCurrentDelta @unit(mA) = 0 mA;
		double txCurrent @unit(mA) = 17 mA;
		double setupRxCurrent @unit(mA) = 8.2mA;
		double setupTxCurrent @unit(mA) = 8.2mA;
		double rxTxCurrent @unit(mA) = 17 mA;
		double txRxCurrent @unit(mA) = 17 mA;
		bool startsOn = default(true);
	
	gates:
		input upperGateIn; // from upper layers
		input radioIn; // to receive AirFrames
		output upperGateOut; // to upper layers
		output upperControlOut; // control connection
		input upperControlIn;

	submodules:
		nicController: NicController {
			parameters:
				@display("p=111,53;i=block/wrxtx");
		}
		phy: myPhyLayerControl {
			parameters:
				decider = xmldoc("config.xml");
				analogueModels = default(xmldoc("config.xml"));
				//publishRSSIAlways = false;
				sensitivity = -100 dBm;
				headerLength = 48 bit; // ieee 802.15.4
				thermalNoise = -110 dBm;
				maxTXPower = 1 mW;
				initialRadioState = 0;
				// From TI CC1100 datasheet rev. C
				timeSleepToRX = 0.00108 s;
				timeSleepToTX = 0.00108 s;
				timeRXToTX = 0.000010 s;
				timeTXToRX = 0.000022 s;
				timeRXToSleep = 0 s;
				timeTXToSleep = 0 s;
				@display("p=96,236;i=block/wrxtx");
		}
		mac: csmaControl {
			parameters:
				rxSetupTime = 0.00108 s;
				bitrate = 250000 bps;

				@display("p=96,87;i=block/layer");

        }
        
	connections:

		radioIn --> phy.radioIn;
	
		// phy to mac: data goes here
		phy.upperGateIn <-- { @display("m=m,25,0,25,0"); } <-- mac.lowerGateOut;
		phy.upperGateOut --> { @display("m=m,73,0,50,50"); } --> mac.lowerGateIn;
	
		// phy to mac: control goes here:
		phy.upperControlIn <-- { @display("m=m,25,0,25,0"); } <-- mac.lowerControlOut;
		phy.upperControlOut --> { @display("m=m,73,0,50,50"); } --> mac.lowerControlIn;
	
		// mac to control: control goes here
		mac.upperControlIn <-- nicController.lowerControlOut;
		mac.upperControlOut --> nicController.lowerControlIn;
	
		// mac to control: data goes here
		mac.upperGateOut --> nicController.lowerGateIn;
		mac.upperGateIn <-- nicController.lowerGateOut;
	
		// control to upper layers: data and control
		nicController.upperControlIn <-- upperControlIn;
		nicController.upperControlOut --> upperControlOut;
		nicController.upperGateIn <-- upperGateIn;
		nicController.upperGateOut --> upperGateOut;
}

