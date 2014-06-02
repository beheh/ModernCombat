/*-- Düsternis --*/

//Erweitert die Düsternis im Zusammenhang mit der "Eingeschränkte Sicht"-Regel.

#strict 2

#appendto DARK


/* Dunkelheit bestimmen */

private func PostInitialize()
{
  //Nur eine Dunkelheit
  var count = 1;
  for(var d in FindObjects(Find_ID(GetID()),Find_Exclude(this())))
  {
    count++;
    RemoveObject(d);
  }

  //Dunkelheit setzen
  SetDarkness(count);
}

public func ChooserFinished()
{
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Not(Find_Owner(NO_OWNER))))
  {
    if(!GameCallEx("RejectViewRangeChange",500-GetDarkness(100)))
      SetPlrViewRange(500-GetDarkness(100),pClonk);

    var tmp = AddLightAmbience(80,pClonk);
    tmp->ChangeColor(RGBa(255,255,255,110));
    SetOwner(GetOwner(pClonk), tmp);
    SetVisibility(VIS_Owner,tmp);

    if(!GameCall("NoFlashlight"))
      if(GetDarkness() && IsDark() && GetPlayerType(GetOwner(pClonk)) != C4PT_Script)
        CreateContents(FLSH,pClonk);
  }
}

public func OnClonkRecruitment(object pClonk)
{
  if(FindObject(CHOS)) return;

  if(ObjectCount(GetID()) > 1)
    return ScheduleCall(this,"OnClonkRecruitment",1,0,pClonk);

  if(!GameCallEx("RejectViewRangeChange",500-3*GetDarkness(100)))
    SetPlrViewRange(500-3*GetDarkness(100),pClonk);

  var tmp = AddLightAmbience(80,pClonk);
  tmp->ChangeColor(RGBa(255,255,255,110));
  SetOwner(GetOwner(pClonk), tmp);
  SetVisibility(VIS_Owner,tmp);
}
