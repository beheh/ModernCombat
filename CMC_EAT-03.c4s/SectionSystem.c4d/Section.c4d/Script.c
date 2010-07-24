/*-- Sektionskontrolle --*/

#strict 2

local iScript;
local fStop;

public func Initialize() {
	iScript = 0;
	fStop = false;
	this->~SetupSection();
	ScheduleCall(this, "Count", 10);
}

public func Count() {
	if(fStop) return;
	ObjectCall(this, Format("Script%d", iScript));
	iScript++;
	ScheduleCall(this, "Count", 10);
	return true;
}

public func SectionGoto(int iTo) {
	iScript = iTo;
	return true;
}

public func SectionStop() {
	fStop = 1;
	return true;
}
