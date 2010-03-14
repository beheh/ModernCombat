/*-- Schwerverletzter Clonk --*/


/* Schadenskontrolle */

global func FxDmgCheckDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  if(iDmgEngy > 0) return(iDmgEngy);
  
  var type;
  //Explosion
  if(iCause == 1 || iCause == 33)
    type = DMG_Explosion;
  //Feuer
  else if(iCause == 2 || iCause == 35)
    type = DMG_Fire;
  //Säure (Bio)
  else if(iCause == 38)
    type = DMG_Bio;
  //Melee = Clonk zu Clonk Kampf
  else if(iCause == 34 || iCause == 40)
    type = DMG_Melee;

  //Ziel übergeben welcher Schadenstyp gemacht wurde
  if(type)
  {
   pTarget->~LastDamageType(type);

   var red = pTarget->~OnDmg(iDmgEngy/100,type); 
   //reduzierten Schaden berechnen
   var dmg;
   dmg = (100-red)*iDmgEngy;
   iDmgEngy = dmg/100;
  }

  if(pTarget->~IsClonk() && !ObjectCount(NOFD) && !IsFakeDeath(pTarget))
  {
   if(type)
    pTarget->HurtSounds(iDmgEngy,type);

   if(GetEnergy(pTarget) <= -iDmgEngy/1000)
   {
    FakeDeath(pTarget);
    return(0);
   }
  }
  return(iDmgEngy);
}

/* Faketot */

func FakeDeathHP(){return(20);}

/* Entgültiger Tod */

func Death(object pTarget)
{
  if(IsFakeDeath())
  {
   SetPhase(5);//Fallanimation überspringen
   Sound("Death");
  }
  else
  {
   Sound("ClonkDie*.ogg");
  }
  //Verschwinden
  FadeOut(this());

  //Leiche "auswerfen" und ausfaden lassen 
  Exit(0,0,GetObjHeight(pTarget)/2);

  //Fake Death löschen
  RemoveObject(pTarget);
}