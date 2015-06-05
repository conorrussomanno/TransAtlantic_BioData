# transatlantic_biodata
This repo contains all of the code to send OpenBCI biosignals from the Processing GUI via a TCP socket to another computer via the Internet. It will first be tested by hackers at the BCI Montreal and Hack The Brain Amsterdam hackathons, that are happening simultaneously. 

## Files

* **Processing/OpenBCI_GUI_EMG** — this Processing code acts as the EMG receiver and signal processor from the OpenBCI board. It also broadcasts the data over TCP to the machine running **NetBridge_Receiver**.
* **Processing/NetBridge_Receiver** — this Processong code acts as the data receiver. Currently, it maps a pseudo-analog range of values onto the opacity of the background... it will soon send the data over serial to the Arduino with a HHI shield. 
* **Arduino/HHI** — This Arduino code takes the serial data from **NetBridge_Receiver** and triggers the HHI to either send a current or not to the wearer's arm.


## Steps

1. Setup Hamachi (get "IP")
	a. Get LogMeIn Hamachi: [https://secure.logmein.com/products/hamachi/download.aspx](https://secure.logmein.com/products/hamachi/download.aspx)
	* asdf
	* asdf
* OpenBCI_GUI_EMG =>NetBridge Receiver
* NetBridge_Receiver => HHI
* HHI => Hand Setup (adjust values... be careful...)
* TransAtlantic BioData Time!