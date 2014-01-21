package mywsn;

import org.mixim.modules.nic.*;

import org.mixim.base.modules.*;
import org.mixim.modules.power.battery.SimpleBattery;
import org.mixim.modules.power.battery.BatteryStats;

module BaseNode
{
    parameters:
        string applType; //type of the application layer
        string netwType = "BaseNetwLayer"; //type of the network layer
        string mobType = "BaseMobility"; //type of the mobility module
        @display("bgb=,,white,,");
    gates:
        input radioIn; // gate for sendDirect
    submodules:
        batteryStats: BatteryStats {
            parameters:
                @display("p=208,166;i=block/circle");
                debug = false;
                detail = false;
                timeSeries = false;
        }
        battery: myBattery {
            parameters:
                @display("p=208,101;i=block/control");
                debug = true;
                nominal = 99999mAh;
                capacity = 99999mAh;
                voltage = 3.3V;
                resolution = 1s;
                publishDelta = 1;
                publishTime = 0;
                numDevices = 1;
        }
        utility: BaseUtility {
            parameters:
                @display("p=130,38;b=24,24,rect,black;i=block/blackboard");
        }
        arp: BaseArp {
            parameters:
                @display("p=130,101;b=24,24,rect,blue;i=block/process");
        }
        mobility: <mobType> like IBaseMobility {
            parameters:
                @display("p=130,166;i=block/cogwheel");
        }
        appl: <applType> like IBaseApplLayer {
            parameters:
                @display("p=59,38;i=app");
        }
        net: <netwType> like IBaseNetwLayer {
            parameters:
                @display("p=60,101;i=block/layer");
                stats = false;
                headerLength = 32bit;
        }
        nic: myNIC {
            parameters:
                @display("p=60,166;i=block/ifcard");

                //################ PhyLayer parameters #####################
                phy.usePropagationDelay = false;
                phy.useThermalNoise = false;

                //################ MAC layer parameters ####################
                mac.queueLength = 5;
                mac.headerLength = 24bit;
                //mac.bitrate = 15360bps;
                mac.txPower = 100mW;
                mac.stats = true;
                mac.trace = true;

                mac.ccaDetectionTime = 0.0005s;
		//mac.aTurnaroundTime = 0s; //no radio switch times
		//mac.rxSetupTime = 0s; //no radio switch times

                mac.backoffMethod = "linear";
                mac.macMaxCSMABackoffs = 14;
                mac.contentionWindow = 20;
                mac.aUnitBackoffPeriod = 0.04s;

                // MAC Ack settings (disabled)
                mac.useMACAcks = false;
                mac.ackLength = 0bit;
                mac.macMaxFrameRetries = 0;
                mac.macAckWaitDuration = 0s;
                mac.sifs = 0s;

                mac.macMinBE = 0; //only used for exponential backoffs
                mac.macMaxBE = 0; //only used for exponential backoffs

        }
    connections:
        nic.upperGateOut --> net.lowerGateIn;
        nic.upperGateIn <-- net.lowerGateOut;
        nic.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> net.lowerControlIn;
        nic.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- net.lowerControlOut;

        net.upperGateOut --> appl.lowerGateIn;
        net.upperGateIn <-- appl.lowerGateOut;
        net.upperControlOut --> { @display("ls=red;m=m,70,0,70,0"); } --> appl.lowerControlIn;
        net.upperControlIn <-- { @display("ls=red;m=m,70,0,70,0"); } <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;

}
