
function updatewaninfo (req)
{

	var WanInfo=req.responseText;
	if ( WanInfo != "" && WanInfo != null ) {
		WanInfos = WanInfo.split('||');
		if ( WanInfos.length == 44 ) {

			var i = 0;
				
			i++;//DSL Status
			i++;//WAN Status 

			if (WanInfos[0] == 'CONNECTED')
			  document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_Connected.png" alt="Connected" width="152" height="25" /><p class="connected">' + WanInfos[2] + '/' + WanInfos[3] + ' Mbps</p>';
			else if (WanInfos[0] == 'CONNECTING')
				document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_Connecting.png" alt="Connecting" width="152" height="25" /><p class="notconnected">N/A Mbps</p>';
			else
				document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_notConnected.png" alt="Not Connected" width="152" height="25" /><p class="notconnected">N/A Mbps</p>';

			if (WanInfos[1] == 'CONNECTED')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_Connected.png" alt="Connected" width="152" height="25" />';
			else if (WanInfos[1] == 'WALLED GARDEN')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_walledgarden.png" alt="Walled Garden" width="152" height="25" />';
			else if (WanInfos[1] == 'CONNECTING')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_Connecting.png" alt="Connecting" width="152" height="25" />';
			else
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_notConnected.png" alt="Not Connected" width="152" height="25" />';

			document.getElementById("dspeed").innerHTML = WanInfos[i++];
			document.getElementById("uspeed").innerHTML = WanInfos[i++];
			document.getElementById("DSLMODE").innerHTML = WanInfos[i++];
			document.getElementById("DSLMODEDECT").innerHTML = WanInfos[i++];
			document.getElementById("DSLUPTIME").innerHTML = WanInfos[i++];

			var Retrain = WanInfos[i++];
			var intValue = parseInt(Retrain);
			if ( isNaN(intValue) ) {
				document.getElementById("RETRAINS").innerHTML = 'N/A';
			}
			else if ( intValue < 7 ) {
				document.getElementById("RETRAINS").innerHTML = Retrain;
			}
			else {
				document.getElementById("RETRAINS").innerHTML = '<font color="#FF0000"><strong>'+Retrain+'</strong></font>';
			}

			var RetrainCurr24H = WanInfos[i++];
			intValue = parseInt(RetrainCurr24H);
			if ( isNaN(intValue) ) {
				document.getElementById("RETRAINS24HR").innerHTML = 'N/A';
			}
			else if ( intValue < 3 ) {
				document.getElementById("RETRAINS24HR").innerHTML = RetrainCurr24H;
			}
			else {
				document.getElementById("RETRAINS24HR").innerHTML = '<font color="#FF0000"><strong>'+RetrainCurr24H+'</strong></font>';
			}

			document.getElementById("linkFailLPR").innerHTML = WanInfos[i++];
			document.getElementById("linkFailLOS").innerHTML = WanInfos[i++];
			document.getElementById("linkFailLOM").innerHTML = WanInfos[i++];
			document.getElementById("linkTrainErr").innerHTML = WanInfos[i++];
			document.getElementById("DSLUAS").innerHTML = WanInfos[i++];

			var l2Intf = WanInfos[i++];
			if ( l2Intf == 1 ) {
				document.getElementById("TRANSMODE").innerHTML = 'ATM';
			}
			else {
				document.getElementById("TRANSMODE").innerHTML = 'PTM';
			}

			document.getElementById("VPIVCI_VLAN").innerHTML = WanInfos[i++];

			document.getElementById("PRIORITY").innerHTML = WanInfos[i++];
			document.getElementById("SERVICETYPE").innerHTML = WanInfos[i++];

			//DSL POWER
			document.getElementById("DSLSNRDOWN").innerHTML = WanInfos[i++];
			document.getElementById("DSLSNRUP").innerHTML = WanInfos[i++];
			document.getElementById("DSLATTENDOWN").innerHTML = WanInfos[i++];
			document.getElementById("DSLATTENUP").innerHTML = WanInfos[i++];
			document.getElementById("DSLPOWERDOWN").innerHTML = WanInfos[i++];
			document.getElementById("DSLPOWERUP").innerHTML = WanInfos[i++];
			//DSL POWER

			document.getElementById("dsldpkt").innerHTML = WanInfos[i++];
			document.getElementById("dslupkt").innerHTML = WanInfos[i++];
			document.getElementById("dslderrpkt").innerHTML = WanInfos[i++];
			document.getElementById("dsluerrpkt").innerHTML = WanInfos[i++];
			document.getElementById("dslRxByte1Day").innerHTML = WanInfos[i++];
			document.getElementById("dslTxByte1Day").innerHTML = WanInfos[i++];
			document.getElementById("dslRxByteTotal").innerHTML = WanInfos[i++];
			document.getElementById("dslTxByteTotal").innerHTML = WanInfos[i++];
			document.getElementById("dsldDiscard30M").innerHTML = WanInfos[i++];
			document.getElementById("dsluDiscard30M").innerHTML = WanInfos[i++];

			document.getElementById("CHANNELTYPE_NEAR").innerHTML = WanInfos[i++];
			document.getElementById("CHANNELTYPE_FAR").innerHTML = WanInfos[i++];
			document.getElementById("CRC_NEAR").innerHTML = WanInfos[i++];
			document.getElementById("CRC_FAR").innerHTML = WanInfos[i++];
			document.getElementById("CRC_NEAR30M").innerHTML = WanInfos[i++];
			document.getElementById("CRC_FAR30M").innerHTML = WanInfos[i++];
			document.getElementById("FEC_NEAR").innerHTML = WanInfos[i++];
			document.getElementById("FEC_FAR").innerHTML = WanInfos[i++];
			document.getElementById("FEC_NEAR30M").innerHTML = WanInfos[i++];
			document.getElementById("FEC_FAR30M").innerHTML = WanInfos[i++];
		}
	}

	setTimeout('updateValue_take()', 5000);
}

function updateValue_take()
{
	var objXMLHTTP = null; 

	if (window.XMLHttpRequest)  { 
		objXMLHTTP=new XMLHttpRequest(); 
	}// code for IE 
		else if (window.ActiveXObject)  { 
		objXMLHTTP=new ActiveXObject("Microsoft.XMLHTTP"); 
	}

	if ( objXMLHTTP != null ) {
		objXMLHTTP.open("GET","./GetDslStatus.html",true);
		objXMLHTTP.setRequestHeader("If-Modified-Since","0");
		objXMLHTTP.send(null);
    objXMLHTTP.onreadystatechange = function() 
    {
			if(objXMLHTTP.readyState == 4)
      	updatewaninfo(objXMLHTTP);
    }
	}
}
//__QWEST__,jhenyang, sync with WriteConnectionStatus().
setTimeout('updateValue_take()', 5000);
