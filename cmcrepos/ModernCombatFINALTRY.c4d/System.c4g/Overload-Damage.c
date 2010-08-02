/* Globale Schadensfunktionen */

#strict 2

global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer, id idKillIcon)
{
  if(!pTarget)
    if(!(pTarget = this))
      return;
  if(!iPrecision)
    iPrecision = 10;

  var dmg, dmgdealer = dmgplayer-1;

  if(dmgdealer < 0)
  	dmgdealer = GetController(pTarget);

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  dmg = iDmg*(100-red)*iPrecision;

  if (!dmg || red == 100) return;

  // Killer setzen
  if(this && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
    SetKiller(dmgdealer, pTarget);
  
  if(GetOCF(pTarget) & OCF_CrewMember)
  {
    if(!idKillIcon && this)//Kein Killicon?
    {
      idKillIcon = this->~GetKillIcon();
      
      if(!idKillIcon)
        idKillIcon = GetID(this);
      
      if(idKillIcon->~IsClonk())
        idKillIcon = 0;      
    }
    
    if(!idKillIcon)
      {
        if(iType)
        {
          if(iType == DMG_Fire)
            idKillIcon = GSAM;
          else if(iType == DMG_Explosion)
            idKillIcon = BOOM;
          else if(iType == DMG_Energy)
            idKillIcon = ENAM;
          else if(iType == DMG_Bio)
            idKillIcon = GLOB;
          else if(iType == DMG_Projectile)
            idKillIcon = STAM;
        }
        else
          idKillIcon = KAMB;
      }
    
    pTarget->~KillIcon(idKillIcon);
  }

  //Schaden machen
  var pFrom;
  if(pTarget != this)
    pFrom = this;
  if(!pFrom) pFrom = GetCrew(dmgdealer);

  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);
  if(GetCategory(pTarget) & C4D_Living)
    return DoEnergy(-dmg, pTarget, true, 0, dmgplayer);
  return DoDamage(dmg/1000,pTarget);
}
