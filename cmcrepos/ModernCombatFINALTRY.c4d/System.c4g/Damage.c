/* Globale Schadensfunktionen */

#strict


global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer, id idKillIcon)
{
  if(!pTarget)
    if(!(pTarget = this()))
      return(0);
  if(!iPrecision)
    iPrecision = 10;

  var dmg;
  var dmgdealer = dmgplayer-1;
  if(dmgdealer < 0)
  	dmgdealer = GetController(pTarget);

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  // reduction
  dmg = iDmg*(100-red)*iPrecision;

  // Killer setzen
  if(this() && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
    SetKiller(dmgdealer, pTarget);
  
  if(GetOCF(pTarget) & OCF_CrewMember)
  {
    if(!idKillIcon)//Kein Killicon?
    {
      if(this())
        idKillIcon = this()->~GetKillIcon();
      
      if(this() && !idKillIcon)
        idKillIcon = this()->GetID();
      
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
    }
    
    pTarget->~KillIcon(idKillIcon);
  }

  //Schaden machen
  var pFrom;
  //if(pTarget != this())
    pFrom = this();
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);
  if(GetCategory(pTarget) & C4D_Living)
    return(DoEnergy(-dmg,pTarget, true));
  return(DoDamage(dmg/1000,pTarget));
}
