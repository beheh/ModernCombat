/*-- Sektionskontrolle --*/

#strict 2

static iScript;

public func IsSection() {return true;} //Sektionscallback

public func Initialize() {
	iScript = 0;
	this->~SetupSection();
	AddEffect("SectionCount", this, 25, 10, this);
}

public func GetScript() {
	return iScript;
}

global func FxSectionCountTimer(pTarget) {
	var iScript = pTarget->GetScript();
	if(iScript < 0) return;
	pTarget->SectionGoto(iScript+1);
	ObjectCall(pTarget, Format("Script%d", iScript));
	return FX_OK;
}

public func SectionGoto(int iTo) {
	iScript = iTo;
	return true;
}

public func SectionStop() {
	iScript = -1;
	return true;
}

public func SectionUnload() {
	RemoveObject();
}
