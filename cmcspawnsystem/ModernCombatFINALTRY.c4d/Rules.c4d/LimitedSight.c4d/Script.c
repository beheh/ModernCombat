/*-- Beschränkte Sicht --*/

#strict 2

public func IsChooseable()	{return 1;}	//Kann mittels des Spielzielauswählers ausgewählt werden


protected func Activate(int iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

public func ChooserFinished()
{
  for(var pObj in FindObjects(Find_OCF(OCF_CrewMember), Find_Func("IsClonk")))
  {
    if(!GetEffect("Sight",pObj))
      AddEffect("Sight", pObj, 10, 1, this, GetID());
  }
}

protected func Destruction()
{
  var i, iEffect;
  for(var pObj in FindObjects(Find_OCF(OCF_CrewMember), Find_Func("IsClonk")))
  {
    i = GetEffectCount(0,pObj);
    while(i--)
      if(iEffect = GetEffect("Sight", pObj, i))
        RemoveEffect(0, pObj, iEffect);
  }
}

public func RejectViewRangeChange(int iNewRange)
{
  return true;
}

public func OnClonkRecruitment(object pClonk, int iPlr)
{
  if(FindObject(CHOS)) return;

  if(!GetEffect("Sight",pClonk))
    AddEffect("Sight", pClonk, 10, 1, this, GetID());
}

/* Funktionalität */

static const L_AS_Distance = 200;
static const L_AS_Range = 50;

private func SightHelperCnt()
{
  return(L_AS_Distance-L_AS_Range)/L_AS_Range;
}

public func FxSightStart(object pTarget,int iEffectNumber, int iTemp)
{
  //Status sichern
  EffectVar(1,pTarget,iEffectNumber) = GetPlrFogOfWar(GetOwner(pTarget));
  EffectVar(2,pTarget,iEffectNumber) = GetObjPlrViewRange(pTarget);

  //Neuen Status einstellen
  SetFoW(true,GetOwner(pTarget)); 
  SetPlrViewRange(L_AS_Range,pTarget);
  Schedule(Format("SetPlrViewRange(%d)",L_AS_Range),1,0,pTarget);

  //Sichtobjekte erstellen
  var aSight = CreateArray(SightHelperCnt());
  for(var i = 0; i < SightHelperCnt(); i++)
  {
    if(aSight[i]) RemoveObject(aSight[i]);//Reset.
    aSight[i] = CreateObject(SHLP,0,0,GetOwner(pTarget));//Erstellen.
    aSight[i]->SetController(GetOwner(pTarget));
  }

  EffectVar(0,pTarget,iEffectNumber) = aSight;

  EffectCall(pTarget,iEffectNumber,"Timer",0);
}

public func FxSightTimer(object pTarget,int iEffectNumber, int iEffectTime)
{
  var aSight = EffectVar(0,pTarget,iEffectNumber);

  if(!Contained(pTarget) && GetAlive(pTarget))
  {
    var angle;
    if(!pTarget->~IsAiming())
    {
      angle = 90;
      if(GetDir(pTarget) == DIR_Left)
        angle = 270;
    }
    else
    {
      angle = pTarget->~AimAngle();
    }

    var mx = +Sin(angle,L_AS_Distance),
        my = -Cos(angle,L_AS_Distance),
        ml = 0;

    for(var i = 0; i < GetLength(aSight); i++)
    {
      ml += L_AS_Range;
      SetPlrViewRange(L_AS_Range,aSight[i]);
      aSight[i]->SetPosition(pTarget->GetX() + (mx*ml/L_AS_Distance), pTarget->GetY() + (my*ml/L_AS_Distance));
    }
  }
  else
  //Spezielle Helikopter-Sicht
  if(GetID(Contained(pTarget))->~IsHelicopter())
  {
    var angle, helicopter = Contained(pTarget); 

    //Piloten und Passagiere
    if(pTarget == helicopter->~GetPilot() || pTarget == helicopter->~GetPassenger1() || pTarget == helicopter->~GetPassenger2())
    {
      angle = 90;
      if(GetDir(helicopter) == DIR_Left)
        angle = 270;

      var rotation = LocalN("rotation", helicopter);
      angle += rotation;
    }
    //Schütze
    else if(pTarget == helicopter->~GetGunner())
    {
      var weapon = helicopter->GetRopeAttach();
      angle = weapon->AimAngle();
    }
    else if(pTarget == helicopter->~GetCoordinator())
    {
      var weapon = LocalN("pRocketStation", helicopter);
      angle = weapon->AimAngle();
    }

    var mx = +Sin(angle,L_AS_Distance),
    my = -Cos(angle,L_AS_Distance),
    ml = 0;	

    for(var i = 0; i < GetLength(aSight); i++)
    {
      ml += L_AS_Range;
      SetPlrViewRange(L_AS_Range,aSight[i]);
      aSight[i]->SetPosition(pTarget->GetX() + (mx*ml/L_AS_Distance), pTarget->GetY() + (my*ml/L_AS_Distance));
    }
  }
  else
  {
    for(var o in aSight)
    {
      o->SetPosition(GetX(pTarget), GetY(pTarget));
      SetPlrViewRange(0,o); 
    }
  }
}

public func FxSightStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  for(var o in EffectVar(0,pTarget,iEffectNumber))
    RemoveObject(o);

  SetFoW(EffectVar(1,pTarget,iEffectNumber),GetOwner(pTarget)); 
  SetPlrViewRange(EffectVar(2,pTarget,iEffectNumber),pTarget);
}