/*-- Sektionskontrolle --*/

#strict 2

local iScript;

public func Initialize() {
	iScript = 0;
	this->~SetupSection();
	ScheduleCall(this, "Count", 10);
}

public func Count() {
	if(GetID() != SSY2)
	if(iScript < 0) return;
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
	iScript = -1;
	return true;
}
