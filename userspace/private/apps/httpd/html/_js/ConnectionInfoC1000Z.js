var sessiontime = 'N/A';
var pppExist = '0';
var pppusername = '';
var linkStatus = 'NOT CONNECTED';
var g_protocol = '';
function updateConnectionInfo (req)
{
	var ConnectionInfo=req.responseText;
	if ( ConnectionInfo != "" && ConnectionInfo != null ) {
		ConnectionInfos = ConnectionInfo.split('|');
		if ( ConnectionInfos.length == 16 ) {
		//linespeeddown|linespeedup|PPPTYPE|USERNAME|broadban|ispstats|Encap|WANIP|dns1|dns2|session time|PPPDEXIST|
			pppExist = ConnectionInfos[11];

			sessiontime = ConnectionInfos[10];
			pppusername = ConnectionInfos[3];
			g_protocol = ConnectionInfos[2];
			document.getElementById("dspeed").innerHTML = ConnectionInfos[0];
			document.getElementById("uspeed").innerHTML = ConnectionInfos[1];

			if (ConnectionInfos[2] == null ) {
				document.getElementById("ISPPROTO").innerHTML = 'N/A';
			}
			else {
				document.getElementById("ISPPROTO").innerHTML = ConnectionInfos[2];
			}

			if (ConnectionInfos[3] == null || ConnectionInfos[3] == 'N/A' ) {
				document.getElementById("USERNAME").innerHTML = 'N/A';
			}
			else {
				document.getElementById("USERNAME").innerHTML = ConnectionInfos[3];
			}
		
			if (ConnectionInfos[4] == 'CONNECTED')
				document.getElementById("broadband").innerHTML = '<font color="#015F2C"><strong>'+ConnectionInfos[4]+'</strong></font>';
			else if (ConnectionInfos[4] == 'CONNECTING')
				document.getElementById("broadband").innerHTML = '<font color="#D7DB24"><strong>'+ConnectionInfos[4]+'</strong></font>';
			else
				document.getElementById("broadband").innerHTML = '<font color="#FF0000"><strong>'+ConnectionInfos[4]+'</strong></font>';

			if (ConnectionInfos[4] == 'CONNECTED')
				document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_Connected.png" alt="Connected" width="152" height="25" /><p class="connected">' + ConnectionInfos[0] + '/' + ConnectionInfos[1] + ' Mbps</p>';
			else if (ConnectionInfos[4] == 'CONNECTING')
				document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_Connecting.png" alt="Connecting" width="152" height="25" /><p class="notconnected">N/A Mbps</p>';
			else
				document.getElementById("broadband2").innerHTML = '<img src="../_images/connectionStatus_notConnected.png" alt="Not Connected" width="152" height="25" /><p class="notconnected">N/A Mbps</p>';

			linkStatus = ConnectionInfos[4];

			if (ConnectionInfos[5] == 'CONNECTED')
				document.getElementById("ISPSTATS").innerHTML='<font color="#015F2C"><strong>'+ConnectionInfos[5]+'</strong></font>';
			else if (ConnectionInfos[5] == 'WALLED GARDEN')
				document.getElementById("ISPSTATS").innerHTML='<font color="#EDAB12"><strong>'+ConnectionInfos[5]+'</strong></font>';	
			else if (ConnectionInfos[5] == 'CONNECTING')
				document.getElementById("ISPSTATS").innerHTML='<font color="#D7DB24"><strong>'+ConnectionInfos[5]+'</strong></font>';	
			else
				document.getElementById("ISPSTATS").innerHTML='<font color="#FF0000"><strong>'+ConnectionInfos[5]+'</strong></font>';

			if (ConnectionInfos[5] == 'CONNECTED')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_Connected.png" alt="Connected" width="152" height="25" />';
			else if (ConnectionInfos[5] == 'WALLED GARDEN')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_walledgarden.png" alt="Walled Garden" width="152" height="25" />';
			else if (ConnectionInfos[5] == 'CONNECTING')
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_Connecting.png" alt="Connecting" width="152" height="25" />';
			else
				document.getElementById("ISPSTATS2").innerHTML='<img src="../_images/connectionStatus_notConnected.png" alt="Not Connected" width="152" height="25" />';

				

			var wanip = '<%ejGetOther(dslinfo, WANIP)%>';
			var wanStatus = '<%ejGetOther(sysInfo, defaultWan)%>';
		
			if ( ConnectionInfos[7] == '' || ConnectionInfos[7] == '&nbsp;' || ConnectionInfos[7] == '0.0.0.0' || ConnectionInfos[7] == 'N/A' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') ) {
				document.getElementById("ModemIP").innerHTML='N/A';
			}
			else {
				document.getElementById("ModemIP").innerHTML="<font color=\"#015F2C\"><strong>"+ConnectionInfos[7]+"</strong></font>";
			}
			
			if ( ConnectionInfos[8] == '&nbsp' || ConnectionInfos[8] == '0.0.0.0' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') || ConnectionInfos[2] == 'Transparent Bridging' ) {
				document.getElementById("ModemDNS1").innerHTML='N/A';
			}
			else {
				document.getElementById("ModemDNS1").innerHTML=ConnectionInfos[8];
			}
			
			if ( ConnectionInfos[9] == '&nbsp' || ConnectionInfos[9] == '0.0.0.0' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') || ConnectionInfos[2] == 'Transparent Bridging' ) {
				document.getElementById("ModemDNS2").innerHTML='N/A';
			}
			else {
				document.getElementById("ModemDNS2").innerHTML=ConnectionInfos[9];
			}

			if ( ConnectionInfos[12] == '' || ConnectionInfos[12] == '&nbsp;' || ConnectionInfos[12] == '0.0.0.0' || ConnectionInfos[12] == 'N/A' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') ) {
				document.getElementById("ModemIPv6addr").innerHTML='N/A';
			}
			else {
            document.getElementById("ModemIPv6addr").innerHTML="<font color=\"#015F2C\"><strong>"+ConnectionInfos[12]+"</strong></font>";
			}

			if ( ConnectionInfos[13] == '&nbsp' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') || ConnectionInfos[13] == '' || ConnectionInfos[2] == 'Transparent Bridging' || ConnectionInfos[12] == 'N/A' ) {
				document.getElementById("ModemIPv6DNS1").innerHTML='N/A';
			}
			else {
				document.getElementById("ModemIPv6DNS1").innerHTML=ConnectionInfos[13];
			}
			
			if ( ConnectionInfos[14] == '&nbsp' || (ConnectionInfos[5] != 'CONNECTED' && ConnectionInfos[5] != 'WALLED GARDEN') || ConnectionInfos[14] == '' || ConnectionInfos[2] == 'Transparent Bridging' || ConnectionInfos[12] == 'N/A' ) {
				document.getElementById("ModemIPv6DNS2").innerHTML='N/A';
			}
			else {
				document.getElementById("ModemIPv6DNS2").innerHTML=ConnectionInfos[14];
			}

			if (ConnectionInfos[15] == null ) {
				document.getElementById("IPCONNTYPE").innerHTML = 'N/A';
			}
			else {
				document.getElementById("IPCONNTYPE").innerHTML = ConnectionInfos[15];
			}
// done hiding -->
		}
	}
	protocolChange();
	//__QWEST__,jhenyang, sync with WriteConnectionStatus().
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
		objXMLHTTP.open("GET","./GetConnectionInfo.html",true);
	objXMLHTTP.setRequestHeader("If-Modified-Since","0");
    objXMLHTTP.onreadystatechange = function() 
    {
			if(objXMLHTTP.readyState == 4)
      	updateConnectionInfo(objXMLHTTP);
    }
		objXMLHTTP.send(null);
	}
}

setTimeout('updateValue_take()', 5000);
