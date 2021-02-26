
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
		return ["hc", "tank", "water", "solarHc", "inside", "outside", "outside2", "tankReal"];
	}
	
	getValueUnits() {
		return ["°", "°", "°", "°", "°", "°", "°", "°"];
	}
	
	getValueLabels() {
		return ["HeatChanger", "Tank", "Water", "Solar HC", "Inside", "Outside", "Outside", "Tank"];
	}
	
	getValueDefaults() {
		return [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0];
	}
}

/*
class PvController extends ControllerBase {
	postInit() {
		this.values["power"].value = ko.computed(function() {
			return (this.values["mamps"].value() * 0.230).toFixed(2);
		}, this);
	}

	getValueNames() {
		return ["mamps", "temp", "hum", "power"];
	}
	
	getValueUnits() {
		return ["mAmps", "°", "%", "W"];
	}
	
	getValueLabels() {
		return ["mAmps", "Temp", "Humidity", "Power"];
	}
	
	getValueDefaults() {
		return [0, 0, 0, 0];
	}
}*/

class CurrentController extends ControllerBase {
	getValueNames() {
		return ["main_basement", "pv"];
	}
	
	getValueUnits() {
		return ["mAmp", "mAmp"];
	}
	
	getValueLabels() {
		return ["Basement", "PV"];
	}
	
	getValueDefaults() {
		return [0, 0];
	}
}

class HumidityController extends ControllerBase {
	getValueNames() {
		return ["outside", "outside2"];
	}
	
	getValueUnits() {
		return ["%", "%"];
	}
	
	getValueLabels() {
		return ["Solar", "Outside"];
	}
	
	getValueDefaults() {
		return [0.0, 0.0];
	}
}

class BrightnessController extends ControllerBase {
	getValueNames() {
		return ["solar", "outside"];
	}
	
	getValueUnits() {
		return ["", ""];
	}
	
	getValueLabels() {
		return ["Solar", "Outside"];
	}
	
	getValueDefaults() {
		return [0, 0];
	}
}

class PowerController extends ControllerBase {
	getValueNames() {
		return ["main", "pv", "mainBasement"];
	}
	
	getValueUnits() {
		return ["W", "W", "W"];
	}
	
	getValueLabels() {
		return ["Total", "PV", "Basement"];
	}
	
	getValueDefaults() {
		return [0, 0, 0];
	}
}

class RelayController extends ControllerBase {
	getValueNames() {
		return ["hcPump", "waterPump", "heatingPump", "boiler", "solarPump", "lightOutside"];
	}
	
	getValueUnits() {
		return ["", "", "", "", "", ""];
	}
	
	getValueLabels() {
		return ["HC Pump", "Water Pump", "Heating Pump", "Boiler", "Solar Pump", "Light Outside"];
	}
	
	getValueDefaults() {
		return [false, false, false, false, false, false];
	}
}

class SwitchController extends ControllerBase {
	getValueNames() {
		return ["wintergarden", "pir"];
	}
	
	getValueUnits() {
		return ["", ""];
	}
	
	getValueLabels() {
		return ["Wintergarden", "PIR"];
	}
	
	getValueDefaults() {
		return [false, false];
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
			"tankOffset", "tempExpThreshold", "cloudsExpThreshold", "tankMinTemp", "boilerSchedule", "subExpSubstract",
			"motionSensorBrightnessThreshold"
		];
	}
	
	getValueUnits() {
		return [
			"", "", "°", "°", "", "", 
			"°", "", "", "",
			"", 
			"°", "", "",
			"", "", "", "", "", "°",
			"", "°", "%", "°", "", "°",
			""
		];
	}
	
	getValueLabels() {
		return [
			"Preheat from", "Preheat to", "Preheat HC Temp", "Preheat Water Temp", "Preheat Mode", "Preheat Duration",
			"Heating Temp", "Use toggle", "Heating Mode", "Heating Manual State",
			"Core Host",
			"HC Standby Temp", "Standby From", "Standby To",
			"Month from", "Month to", "Toggle duration", "Heating from", "Heating to", "Min Tank Temp",
			"Tank Temp Offset", "Temp Expected Threshold", "Clouds Expected Threshold", "Tank Min Temp", "Boiler Schedule", "Substract target temp if sun expected",
			"Brightness Threshold"
		];
	}
	
	getValueDefaults() {
		return [
			7, 9, 26.0, 24.0, 0, 600,
			22, true, 0, false,
			"",
			26.0, 9, 21,
			9, 5, 60000, 6, 21, 30,
			0, 10, 50, 45, "-15 -15 -15 -15 -5 0 5 5 5 5 -5 -5 -5 -5 -5 -5 0 0 0 0 0 -5 -15 -15", 10,
			10000
		];
	}
}

class SSCController extends ControllerBase {
	getValueNames() {
		return ["", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "sscStatusHk1", "", "", "", "", "sscStatusBoiler", "", "", "", ""];
	}
	
	getValueUnits() {
		return ["", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""];
	}
	
	getValueLabels() {
		return ["SmartHome Type", "4", "5", "6", "7", "Error Code", "HK1 Target Temp", "HK2 Target Temp", "11", "Water Target Temp", "HK1 Max Temp", "MK2 Max Temp", "Error Code Controller", "Error Code Solar", "Error Code Mod2", "Error Code Mod3", "Error Code PMI", "Error Code Pellet TI3", "Error Code Pellet Touch", "Solar Power Current", "Solar Power Day", "Solar Power Total", "22", "Water Power", "24", "Circulation Power", "26", "Boiler Lifetime", "28", "Boiler Starts", "30", "Pellets Lifetime", "32", "Pellets Consumption", "Status Water", "Status Circulation", "Status HK1", "Status HK2", "38", "Status Solar", "Status Swimming Pool", "Status Boiler", "Status Pellet", "Status Wood", "Buffer Target Temp Top", "Tank Target Temp"];
	}
	
	getValueDefaults() {
		return [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
	}
}