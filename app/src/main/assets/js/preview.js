var READYSTATE_COMPLETED = 4;
var HTTP_STATUS_OK = 200;
var POST = 'POST';
var CONTENT_TYPE = 'content-Type';
var TYPE_JSON = 'application/json';
var COMMAND = 'preview/commands/execute';
var OSC_COMMAND = 'osc/commands/execute';
var OSC_CMD_STAT = 'osc/commands/status';
var status;
var PREVIEW_FORMAT = 2;
var RESIZE_WIDTH = 0;
var RESIZE_QUALITY = 100;
var lastTpId = -1;
function stopLivePreview() {
  var command = {
    name: 'camera.stopPreview',
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      console.log(this.responseText);
    } else {
      console.log('stop live preview failed');
    }
  };
  xmlHttpRequest.open(POST, COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}
function startLivePreview() {
  var _parameters = {
    formatNo: PREVIEW_FORMAT
  };
  var command = {
    name: 'camera.startPreview',
    parameters: _parameters
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      console.log(this.responseText);
    } else {
      console.log('start live preview failed');
    }
  };
  xmlHttpRequest.open(POST, COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}
function updatePreviwFrame(){
  var _parameters = {
    resizeWidth: RESIZE_WIDTH,
    quality: RESIZE_QUALITY
  };
  var command = {
    name: 'camera.getPreviewFrame',
    parameters: _parameters
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      var reader = new FileReader();
      reader.onloadend = function onLoad() {
        var img = document.getElementById('lvimg');
        img.src = reader.result;
      };
      reader.readAsDataURL(this.response);
      repeat();
    }
  };
  xmlHttpRequest.open(POST, COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.responseType = 'blob';
  xmlHttpRequest.send(JSON.stringify(command));
}
function repeat() {
  const d1 = new Date();
  while (true) {
    const d2 = new Date();
    if (d2 - d1 > 30) {
      break;
    }
  }
  updatePreviwFrame();
  updatePreviewStat();
  updateEv();
}

function updatePreviewStat() {
  var command = {};
  command.name = 'camera.getPreviewStat';
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      var responseJson = JSON.parse(this.responseText);
      var elements = document.getElementsByName("preview")
      if (responseJson.results=='on') {
        elements[1].checked = true;
      } else {
        elements[0].checked = true;
      }
    } else {
      console.log('getPreviewStat failed');
    }
  };
  xmlHttpRequest.open(POST, COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}

function setEv(ev) {
  var _exposureCompensation = {
    exposureCompensation: ev
  };
  var _parameters = {
    options: _exposureCompensation
  };
  var command = {
    name: 'camera.setOptions',
    parameters: _parameters
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      console.log(this.responseText);
    } else {
      console.log('setOptions failed');
    }
  };
  xmlHttpRequest.open(POST, OSC_COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}
function updateEv() {
  var _optionNames = '[exposureCompensation]';
  var _parameters = {
    optionNames: _optionNames
  };
  var command = {
    name: 'camera.getOptions',
    parameters: _parameters
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      var responseJson = JSON.parse(this.responseText);
      var elements = document.getElementsByName("ev")
      if (responseJson.results.options.exposureCompensation==-2.0) {
        elements[0].checked = true;
      } else if (responseJson.results.options.exposureCompensation==-1.7) {
        elements[1].checked = true;
      } else if (responseJson.results.options.exposureCompensation==-1.3) {
        elements[2].checked = true;
      } else if (responseJson.results.options.exposureCompensation==-1.0) {
        elements[3].checked = true;
      } else if (responseJson.results.options.exposureCompensation==-0.7) {
        elements[4].checked = true;
      } else if (responseJson.results.options.exposureCompensation==-0.3) {
        elements[5].checked = true;
      } else if (responseJson.results.options.exposureCompensation==0.0) {
        elements[6].checked = true;
      } else if (responseJson.results.options.exposureCompensation==0.3) {
        elements[7].checked = true;
      } else if (responseJson.results.options.exposureCompensation==0.7) {
        elements[8].checked = true;
      } else if (responseJson.results.options.exposureCompensation==1.0) {
        elements[9].checked = true;
      } else if (responseJson.results.options.exposureCompensation==1.3) {
        elements[10].checked = true;
      } else if (responseJson.results.options.exposureCompensation==1.7) {
        elements[11].checked = true;
      } else if (responseJson.results.options.exposureCompensation==2.0) {
        elements[12].checked = true;
      }
    } else {
      console.log('getOptions failed');
    }
  };
  xmlHttpRequest.open(POST, OSC_COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}

function takePicture(){
  var command = {
    name: 'camera.takePicture'
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      console.log(this.responseText);
      var responseJson = JSON.parse(this.responseText);
      lastTpId = responseJson.id;
      setTimeout("watchTpComplete()",100);
      
    } else {
      console.log('setOptions failed');
    }
  };
  xmlHttpRequest.open(POST, OSC_COMMAND, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}
function watchTpComplete(){
  var command = {
    id: lastTpId
  };
  var xmlHttpRequest = new XMLHttpRequest();
  xmlHttpRequest.onreadystatechange = function() {
    if (this.readyState === READYSTATE_COMPLETED &&
      this.status === HTTP_STATUS_OK) {
      console.log(this.responseText);
      var responseJson = JSON.parse(this.responseText);
      if (responseJson.results=='inProgress') {
        setTimeout("watchTpComplete()",100);
      } else {
        startLivePreview();
      }
    } else {
      console.log('setOptions failed');
    }
  };
  xmlHttpRequest.open(POST, OSC_CMD_STAT, true);
  xmlHttpRequest.setRequestHeader(CONTENT_TYPE, TYPE_JSON);
  xmlHttpRequest.send(JSON.stringify(command));
}

