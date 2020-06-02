
function Value(name, value, label, unit) {
	this.name = name;
	this.value = ko.observable(value);
	this.label = label;
	this.unit = unit;
}

class ControllerBase {
	constructor(mqttPath) {
		this.mqttPath = mqttPath;
	}
	
	init() {
		this.values = {};
		
		this.valueNames = this.getValueNames();
		this.valueUnits = this.getValueUnits();
		this.valueLabels = this.getValueLabels();
		this.valueDefaults = this.getValueDefaults();
		
		for (var i=0;i<this.valueNames.length;i++) {
			this.values[this.valueNames[i]] = new Value(this.valueNames[i], this.valueDefaults[i], this.valueLabels[i], this.valueUnits[i]);
		};
	}
	
	postInit() {
	}
	
	getValueNames() {
		console.warn("Must override this method");
	}
	
	getValueUnits() {
		console.warn("Must override this method");
	}
	
	getValueLabels() {
		console.warn("Must override this method");
	}
	
	getValueDefaults() {
		console.warn("Must override this method");
	}

	handleMessage(val_index, messagePayload) {
		console.log(val_index);
		console.log(messagePayload);
		
		if (messagePayload.startsWith("d")) {
			this.updateValue(val_index, parseFloat(messagePayload.substr(1)));
		} else if (messagePayload.startsWith("i")) {
			this.updateValue(val_index, parseInt(messagePayload.substr(1)));
		} else if (messagePayload.startsWith("s")) {
			this.updateValue(val_index, messagePayload.substr(1));
		} else if (messagePayload.startsWith("b")) {
			this.updateValue(val_index, (messagePayload.substr(1)=="1" ? true : false));
		}
	}
	
	updateValue(index, newValue) {
		this.values[this.valueNames[index]].value(newValue);
	}
}

class TempController extends ControllerBase {
	postInit() {
		this.values["tankReal"].value = ko.computed(function() {
			return this.values["tank"].value() + controllers["settings"].values["tankOffset"].value();
		}, this);
	}
	
	getValueNames() {
		return ["hc", "tank", "water", "solarHc", "inside", "tankReal"];
	}
	
	getValueUnits() {
		return ["°", "°", "°", "°", "°", "°"];
	}
	
	getValueLabels() {
		return ["HeatChanger", "Tank", "Water", "Solar HC", "Inside", "Tank"];
	}
	
	getValueDefaults() {
		return [0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
	}
}

class PvController extends ControllerBase {
	getValueNames() {
		return ["mamps", "temp", "hum"];
	}
	
	getValueUnits() {
		return ["mAmps", "°", "%"];
	}
	
	getValueLabels() {
		return ["mAmps", "Temp", "Humidity"];
	}
	
	getValueDefaults() {
		return [0, 0, 0];
	}
}

class RelayController extends ControllerBase {
	getValueNames() {
		return ["hcPump", "waterPump", "heatingPump", "boiler", "solarPump"];
	}
	
	getValueUnits() {
		return ["", "", "", "", ""];
	}
	
	getValueLabels() {
		return ["HC Pump", "Water Pump", "Heating Pump", "Boiler", "Solar Pump"];
	}
	
	getValueDefaults() {
		return [false, false, false, false, false];
	}
}

class InfoController extends ControllerBase {
	getValueNames() {
		return ["sysTime", "sysTemp", "boilerTargetTemp", "sunExpected", "avgTempForecast", "avgCloudsForecast"];
	}
	
	getValueUnits() {
		return ["", "°", "°", "", "", "°", ""];
	}
	
	getValueLabels() {
		return ["System Time", "System Temp", "Boiler Target Temp", "Sun Expected", "Avg Temp Forecast", "Avg Clouds Forecase"];
	}
	
	getValueDefaults() {
		return ["", 0.0, 0.0, false, 0.0, 0.0];
	}
}

class SettingsController extends ControllerBase {
	getValueNames() {
		return [
			"preheatFrom", "preheatTo", "preheatHcTemp", "preheatWaterTemp", "preheatMode", "preheatDuration",
			"heatingTemp", "heatingUseToggle", "heatingMode", "heatingManualState",
			"coreHost", 
			"preheatHcStandbyTemp", "preheatStandbyFrom", "preheatStandbyTo",
			"heatingMonthFrom", "heatingMonthTo", "heatingToggleOnDuration", "heatingHourFrom", "heatingHourTo", "heatingMinTempTank",
			"tankOffset", "tempExpThreshold", "cloudsExpThreshold", "tankMinTemp", "boilerSchedule", "subExpSubstract"		
		];
	}
	
	getValueUnits() {
		return [
			"", "", "°", "°", "", "", 
			"°", "", "", "",
			"", 
			"°", "", "",
			"", "", "", "", "", "°",
			"", "°", "%", "°", "", "°" 
		];
	}
	
	getValueLabels() {
		return [
			"Preheat from", "Preheat to", "Preheat HC Temp", "Preheat Water Temp", "Preheat Mode", "Preheat Duration",
			"Heating Temp", "Use toggle", "Heating Mode", "Heating Manual State",
			"Core Host",
			"HC Standby Temp", "Standby From", "Standby To",
			"Month from", "Month to", "Toggle duration", "Heating from", "Heating to", "Min Tank Temp",
			"Tank Temp Offset", ,"Temp Expected Threshold", "Clouds Expected Threshold", "Tank Min Temp", "Boiler Schedule", "Substract target temp if sun expected"
		];
	}
	
	getValueDefaults() {
		return [
			7, 9, 26.0, 24.0, 0,600,
			22, true, 0, false,
			"",
			26.0, 9, 21,
			9, 5, 60000, 6, 21, 30,
			0, 10, 50, 45, "-15 -15 -15 -15 -5 0 5 5 5 5 -5 -5 -5 -5 -5 -5 0 0 0 0 0 -5 -15 -15", 10
		];
	}
}