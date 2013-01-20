/*-- Grenze --*/

#strict 2

local x,y,xh,yh;
local fAbyss;
local fTeamAllow, iAllowedTeam;


/* Einstellung */

public func Set(int iDir, bool fKeepSpawns, bool fAbyssKill, bool fTeamCheck, int iTeam)
{
  fAbyss = fAbyssKill;

  //Teamspezifisch suchen?
  if(fTeamCheck)
  {
    fTeamAllow = true;
    iAllowedTeam = iTeam;
  }

  if(iDir == 0)
  {
    x  = -GetX();
    y  = -GetY();
    xh = GetX();
    yh = LandscapeHeight();
  }
  if(iDir == 1)
  {
    x  = 0;
    y  = -GetY();
    xh = LandscapeWidth()-GetX();
    yh = LandscapeHeight();
  }
  if(iDir == 2)
  {
    x  = -GetX();
    y  = -GetY();
    xh = LandscapeWidth();
    yh = GetY();
  }
  if(iDir == 3)
  {
    x  = -GetX();
    y  = 0;
    xh = LandscapeWidth();
    yh = LandscapeHeight()-GetY();
  }
  if(!fKeepSpawns)
    for(var obj in FindObjects(Find_InRect(x, y, xh, yh), Find_Func("IsSpawnpoint")))
      RemoveObject(obj);
}

/* Grenz�berschreiter suchen */

private func Check()
{
  var id;
  for(var clonk in FindObjects(Find_InRect(x, y, xh, yh), Find_OCF(OCF_CrewMember)))
    if(!GetEffect("Border", clonk))
    {
      if((fTeamAllow && GetPlayerTeam(GetOwner(clonk)) == iAllowedTeam) || Contained(clonk) && (id = GetID(Contained(clonk))) && (id == TIM1 || id == TIM2 || id == FKDT))
        continue;
      AddEffect("Border", clonk, 50, 35, this);
    }
  for(var flag in FindObjects(Find_InRect(x, y, xh, yh), Find_ID(FLA2), Find_Action("Lost")))
    RemoveObject(flag);
}

/* Effekt */

protected func FxBorderStart(pTarget, iNo, iTemp)
{
  if(iTemp)
    return -1;

  if(fAbyss)
  {
    //Opfer sofort t�ten
    pTarget->~KillIcon(SM10);
    pTarget->~LastDamageType(DMG_Melee);
    Kill(pTarget);

    return -1;
  }

  //Countdown
  EffectVar(0, pTarget, iNo) = 10;
  Sound("Alarm.ogg", 0, pTarget, 0, GetOwner(pTarget) + 1);

  //Hinweisnachricht
  PlayerMessage(GetOwner(pTarget), "@$Warning$", pTarget, EffectVar(0, pTarget, iNo));

  //Overlay
  EffectVar(1, pTarget, iNo) = ScreenRGB(pTarget, 1, 1, -10, false, SR4K_LayerBorder);
}

protected func FxBorderTimer(pTarget, iNo, iTime)
{
  var danger = (GetIndexOf(pTarget, FindObjects(Find_InRect(x, y, xh, yh), Find_OCF(OCF_CrewMember))) != -1);

  //Ziel wieder im Sicheren oder schwerverletzt?
  if(!danger || IsFakeDeath(pTarget))
    return -1;

  if(!EffectVar(0, pTarget, iNo))
  {
    //Opfer t�ten
    pTarget->~KillIcon(SM15);
    pTarget->~LastDamageType(DMG_Projectile);
    Kill(pTarget);
    Sound("BRDR_Fire.ogg", true, pTarget);
    return -1;
  }

  EffectVar(0, pTarget, iNo)--;
  PlayerMessage(GetOwner(pTarget), "@$Warning$", pTarget, EffectVar(0, pTarget, iNo));

  var obj = EffectVar(1, pTarget, iNo);
  if(obj)
  {
    RemoveEffect("IntRGBFade", obj, 0, true);
    obj->~SetAlpha(128);
  }
}

protected func FxBorderStop(pTarget, iNo, iReason, fTemp)
{
  if(!fTemp)
  {
    PlayerMessage(GetOwner(pTarget), "@", pTarget);
    if(EffectVar(1, pTarget, iNo))
      ScreenRGB(pTarget, RGB(0, 0, 1, 128), 0, 10, 0, SR4K_LayerBorder);
  }
}

protected func Destruction()
{
  for(var obj in FindObjects(Find_InRect(x, y, xh, yh), Find_OCF(OCF_CrewMember)))
    if(GetEffect("Border", obj, 0, 4) == this)
      RemoveEffect("Border", obj);
}