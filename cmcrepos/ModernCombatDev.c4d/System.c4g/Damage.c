#strict 2

static const DMG_Melee      = 1;
static const DMG_Projectile = 2;
static const DMG_Fire       = 3;
static const DMG_Explosion  = 4;
static const DMG_Energy     = 5;
static const DMG_Bio        = 6;

global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer, id idKillIcon)
{
  if(!pTarget)
    if(!(pTarget = this))
      return 0;
  if(!iPrecision)
    iPrecision = 10;

  var dmg;
  var dmgdealer = dmgplayer-1;
  if(dmgdealer < 0)
  	dmgdealer = GetController(pTarget);

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  dmg = iDmg*(100-red)*iPrecision;

  // Killer setzen
  if(this && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
    SetKiller(dmgdealer, pTarget);
  
  if(GetOCF(pTarget) & OCF_CrewMember)
  {
    if(!idKillIcon)//Kein Killicon?
    {
      if(this)
        idKillIcon = this->~GetKillIcon();
      
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
  //if(pTarget != this)
    pFrom = this;
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);
  if(GetCategory(pTarget) & C4D_Living)
    return DoEnergy(-dmg,pTarget, true);
  return DoDamage(dmg/1000,pTarget);
}

/*global func CheckEnemy(object pObj,object pObj2)
{
  if(!pObj2)
    pObj2 = this();
  var liv1 = (GetCategory(pObj) & C4D_Living),
      liv2 = (GetCategory(pObj2) & C4D_Living);
      
  //ist pObj ein Lebewesen und lebt es noch?
  if(liv1 && !(GetOCF(pObj) & OCF_Alive))
    return(0);

  //und pObj2? 
  if(liv2 && !(GetOCF(pObj2) & OCF_Alive))
    return(0);

  //eines der beiden Lebewesen?
  if(liv1 || liv2)
    //dann können wir ja sowieso abschießen, wenn FriendlyFire an
    if(FriendlyFire())
      return(1);
      
  if((pObj->~IsBulletTarget() == 2) || (pObj2->~IsBulletTarget() == 2))//-.-
    return(1);

  //sonst noch auf Verfeindung checken
  if(Hostile(GetController(pObj),GetController(pObj2)))
    return(1);

  // noch hackiger gehts nicht oder?
  // das ist kein hack... --boni
  if( (GetController(pObj) == NO_OWNER && GetController(pObj2) != NO_OWNER)
    ||(GetController(pObj2) == NO_OWNER && GetController(pObj) != NO_OWNER))
      return(1);

  return(0);
}*/