/* HUD */

#strict 2

static const BHUD_Ready = 0;
static const BHUD_Warning = 1;
static const BHUD_Error = 2;

local pHelicopter;
local iState;
local pRotation;

public func Initialize() 
{
  SetVisibility(VIS_Owner);
  SetState(BHUD_Ready);
  return true;
}

public func SetState(int iNewState) {
	iState = iNewState;
	if(iState == BHUD_Error)
	{
		SetClrModulation(RGBa(255,0,0,50));
		Sound("DamageWarning", true, 0, 100, GetOwner()+1, +1);
	}
	else {
		Sound("DamageWarning", true, 0, 100, GetOwner()+1, -1);
	}
	if(iState == BHUD_Warning)
	{
		SetClrModulation(RGBa(255,153,0,50));
	}
	if(iState == BHUD_Ready)
	{
		SetClrModulation(RGBa(0,153,255,50));
	}
	return true;
}

public func SetHelicopter(object pNewHelicopter) {
	pHelicopter = pNewHelicopter;
	Align();
	return true;
}

public func DamageRecieved()
{
	iState = BHUD_Error;
  return true;
}

protected func Align() {
	SetPosition(BoundBy(GetX(pHelicopter), GetDefWidth(GetID())/2, LandscapeWidth()-GetDefWidth(GetID())/2),
		          BoundBy(GetY(pHelicopter), GetDefHeight(GetID())/2, LandscapeHeight()-GetDefHeight(GetID())/2));
	return true;
}

protected func Timer()
{
	if(!pHelicopter || !pHelicopter->GetPilot()) {
		RemoveObject();
		return true;
	}
	Align();
	//Rotation anzeigen
	if(!pRotation)
	{
		pRotation = CreateObject(BRTN,0,0,GetOwner());
		pRotation->SetOwner(GetOwner());
		pRotation->SetVisibility(VIS_Owner);
		pRotation->SetClrModulation(RGBa(255,204,0,50));
	}
	SetPosition(GetX(), GetY()+56, pRotation);
	pRotation->SetR(GetR(pHelicopter));
	if(GetAction(pHelicopter) == "Turn") {
		pRotation->SetObjDrawTransform(Abs(((GetActTime(pHelicopter)-20)*1000)/40*2),0,0,0,1000,0);
	}
	else {
		pRotation->SetObjDrawTransform(1000,0,0,0,1000,0);
	}
  return true;
}

public func Destruction() {
	if(pRotation) RemoveObject(pRotation);
	return true;
}
