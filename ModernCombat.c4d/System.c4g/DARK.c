/*-- Düsternis --*/

#strict
#appendto DARK

public func ChooserFinished()
{
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_Not(Find_Owner(NO_OWNER))))
  {
    if(!GameCallEx("RejectViewRangeChange",700-3*GetDarkness(100)))
      SetPlrViewRange(700-3*GetDarkness(100),pClonk);

    var tmp = AddLightAmbience(80,pClonk);
    tmp->ChangeColor(RGBa(255,255,255,110));
    tmp->SetOwner(pClonk->GetOwner());
    SetVisibility(VIS_Owner,tmp);

    if(!GameCall("NoFlashlight"))
      if(GetDarkness() && IsDark() && GetPlayerType(pClonk->GetOwner()) != C4PT_Script)
        CreateContents(FLSH,pClonk);
  }
}

public func OnClonkRecruitment(object pClonk)
{
  if(FindObject(CHOS)) return();
  if(ObjectCount(GetID()) > 1)
    return(ScheduleCall(this(),"OnClonkRecruitment",1,0,pClonk));
    
  if(!GameCallEx("RejectViewRangeChange",700-3*GetDarkness(100)))
    SetPlrViewRange(700-3*GetDarkness(100),pClonk);

  var tmp = AddLightAmbience(80,pClonk);
  tmp->ChangeColor(RGBa(255,255,255,110));
  tmp->SetOwner(pClonk->GetOwner());
  SetVisibility(VIS_Owner,tmp);
}