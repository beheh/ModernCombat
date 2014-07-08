/*-- Neues Script --*/

#strict 2

#appendto BRDR

protected func FxBorderStart(pTarget, iNo, iTemp)
{
  if(iTemp)
    return -1;

  if(fAbyss)
  {
    //Opfer sofort töten
    pTarget->~KillIcon(SM10);
    pTarget->~KillAttachment(0, true);
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
    //Opfer töten
    pTarget->~KillIcon(SM15);
    pTarget->~KillAttachment(0, true);
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