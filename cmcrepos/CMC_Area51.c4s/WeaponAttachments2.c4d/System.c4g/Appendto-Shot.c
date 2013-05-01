/*-- Schuss --*/

#strict 2
#appendto SHTX

local iAttachment;

public func BulletStrike(object pObj)
{
  if(pObj)
  {
    if(iAttachment == AT_Anthrax && GetOCF(pObj) & OCF_Alive)
    {
      var anthrax = GetEffect("Anthrax", pObj);
      if(!anthrax)
        anthrax = AddEffect("Anthrax", pObj, 20, 1, 0, GetID());

      EffectVar(0, pObj, anthrax) = iDamage*2/3;	//Gesamtschaden
      EffectVar(1, pObj, anthrax) = iDamage;		//Zeitraum zwischen 2 Schadensausstößen
      EffectVar(2, pObj, anthrax) = shooter;		//Schütze
      EffectVar(3, pObj, anthrax) = wpnid;
    }
    if(iAttachment == AT_Anthrax)
      iDamage = iDamage*2/3;
    DoDmg(iDamage, DMG_Projectile, pObj, 0, 0, 0, iAttachment);
  }
  return true;
}

public func CustomLaunch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iRemoveTime, int Attachment)
{
  fNoTrail = (iTrail == -1);
  iAttachment = Attachment;
  return Launch(iAngle,iSpeed,iDist,iSize,iTrail,iDmg,iRemoveTime);
}

func FxAnthraxStart(object pTarget, int iEffectNumber, int iTemp)
{
  if(iTemp)
    if(GetPhysical("Walk", 2, pTarget) <= GetPhysical("Walk", 1, 0, GetID(pTarget))*5/10)
      return;

  //Lähmung
  //SetPhysical("Walk", GetPhysical("Walk", 2, pTarget)/2, 3, pTarget);
  //SetPhysical("Jump", GetPhysical("Jump", 2, pTarget)/2, 3, pTarget);
  //SetPhysical("Scale", GetPhysical("Scale", 2, pTarget)/2, 3, pTarget);
  //SetPhysical("Hangle", GetPhysical("Hangle", 2, pTarget)/2, 3, pTarget);
  //SetPhysical("Swim", GetPhysical("Swim", 2, pTarget)/2, 3, pTarget);
  ScreenRGB(pTarget,RGBa(45, 230, 25, 190), 80, 3,false, SR4K_LayerMedicament, 200);
}

func FxAnthraxTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Effekt
  //pTarget->CreateParticle("ShockWave",0,0,Random(10),Random(10),5*GetObjHeight(pTarget)+25+Sin(iEffectTime*5,35),RGB(25,230,25),pTarget);
  //Damage machen
  if(!(iEffectTime % EffectVar(1, pTarget, iEffectNumber)))
  {
    DoDmg(1, DMG_Bio, pTarget, 0, GetOwner(EffectVar(2, pTarget, iEffectNumber))+1, EffectVar(3, pTarget, iEffectNumber), AT_Anthrax);
    EffectVar(0, pTarget, iEffectNumber)--;
  }
  
  //Schon leer?
  if(!EffectVar(0, pTarget, iEffectNumber))
    return -1;

  //FakeDeath?
  if(Contained(pTarget) && GetID(Contained(pTarget)) == FKDT)
    return -1;

  //Bildschirm Effekt
  if(!(iEffectTime % 20))
    ScreenRGB(pTarget, RGBa(45, 230, 25, 190), 80, 3, false, SR4K_LayerMedicament, 200);
}

func FxAnthraxStop(object pTarget, no, reason, temp)
{
  if(temp || GetEffect("DragninHeal", pTarget)) return; //Wenn Dragnin grad am Wirken ist, wollen wir das ja nicht entfernen

  //Lähmung rückgängig machen
  //ResetPhysical(pTarget, "Walk");
  //ResetPhysical(pTarget, "Jump");
  //ResetPhysical(pTarget, "Scale");
  //ResetPhysical(pTarget, "Hangle");
  //ResetPhysical(pTarget, "Swim");
}
