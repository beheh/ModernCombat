/*-- AchievementDisplay --*/

#strict 2

local iTime;
local szDesc;
local iSize;

public func Initialize() {
  iTime = 0;
 	iSize = 15;
	szDesc = GetDesc(this);
 	SetVisibility(VIS_Owner);
	SetPosition(-GetDefWidth(GetID())/2-100, -GetDefHeight(GetID())/2-42);
	SetClrModulation(RGBa(255,255,255,255));
	SetObjDrawTransform(10000/iSize,0,-GetDefWidth(GetID())*10000/(2*iSize),0,10000/iSize,-GetDefHeight(GetID())*10000/(2*iSize));
	//Parallax
	Local(0) = 0;
	Local(1) = 0;
}

public func IsAchievement() {
	return true;
}

protected func Fade() {
	iTime++;
	if(iTime < 1*32) {
		SetClrModulation(RGBa(255,255,255,255-(iTime*8)));
		Desc(255-(iTime*8));
		return true;
	}
	if(iTime < 9*32) {
		SetClrModulation(RGBa(255,255,255,0));
		Desc(0);
		return true;
	}
	if(iTime < 13*32) {
		SetClrModulation(RGBa(255,255,255,(iTime-(9*32))*2));
		Desc((iTime-(9*32))*2);
		return true;
	}
	return RemoveObject(this);
}

protected func Desc(int alpha) {
	CustomMessage(Format("<c %x>%s</c>", RGBa(255,255,255,255-alpha),szDesc), this, GetOwner(), (GetDefWidth(GetID())*10/(2*2*iSize)), (GetDefHeight(GetID())*10/(2*iSize))+115, 0, 0, 0, MSG_NoLinebreak);
	return true;
}
