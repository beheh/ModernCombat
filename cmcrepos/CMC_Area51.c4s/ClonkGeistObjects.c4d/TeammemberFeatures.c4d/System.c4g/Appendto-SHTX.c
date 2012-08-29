/*-- Leuchtspur-Feature --*/

#strict 2

#appendto SHTX

local aClr, fDev;

func Construction(object byObj)
{
  //Nichts?
  if(!byObj)
    return;

  //WaffenID?
  wpnid = GetID(byObj);

  //Waffe?
  shooter = GetShooter(byObj);

  //Team?
  if(shooter->GetTeam())
    SetTeam(shooter->GetTeam());
    
  var plr;
  if(IsDeveloper(GetPlayerID(plr = GetOwner(shooter))))
  {
  	var clr = GetPlrColorDw(plr);
  	if(GetTeamConfig(TEAM_TeamColors))
  		clr = GetTeamColor(GetPlayerTeam(plr));
  	
  	aClr = [GetRGBaValue(clr, 1), GetRGBaValue(clr, 2), GetRGBaValue(clr, 3)];
  	fDev = true;
  }
}

public func GlowColor(int iATime)
{
	if(fDev)
		return RGBa(aClr[0], aClr[1], aClr[2], 50);

  return RGBa(255, 190, 0, 50);
}

public func TrailColor(int iATime)
{
  var iPrg = 100 * iATime / iTime;
  if(fDev)
  	return RGBa(aClr[0], aClr[1], aClr[2], iPrg * 2);

	return RGBa(255, 255 - iPrg * 2, 255 - iPrg * 2, iPrg * 2);
}

