/*-- MTP --*/

#strict 2

local ammopoints;

public func HandSize()   	{return 1000;}
public func HandX()     	{return 4000;}
public func HandY()     	{return 10;}
public func IsDrawable() 	{return true;} 

public func MaxAmmoPoints()	{return 200;}
public func StartAmmoPoints()	{return 200;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return(true);}


/* Initialisierung */

protected func Initialize()
{
  //Punkteregeneration
  AddEffect("AMPKRegenerate",this,251,20,this,GetID());
  //Gruppenaufstockung
  AddEffect("AMPKRestocking",this,252,60,this,GetID());
  //Lichteffekt
  AddEffect("AMPKLight",this,250,1,this,GetID());
  ammopoints = StartAmmoPoints();
}

/* Munition entnehmen */

protected func Activate(caller)
{
  //Clonk anhalten
  SetComDir(COMD_Stop, caller);

  //Munitionsmenü erstellen
  CreateMenu(GetID(), caller, this, 0, "$TakeAmmo$", 0, 1);
   AddMenuItem(GetName(0,STAM), "CreateAmmopack", STAM, caller, 50);
   AddMenuItem(GetName(0,GRAM), "CreateAmmopack", GRAM, caller, 12);
   AddMenuItem(GetName(0,MIAM), "CreateAmmopack", MIAM, caller, 4);

  return 1;
}

private func CreateAmmopack(idAmmo) {
  var count, points;
  if (idAmmo == STAM) { count = 50; points = 50; }
  if (idAmmo == GRAM) { count = 12; points = 60; }
  if (idAmmo == MIAM) { count = 4; points = 60; }

  //Zu wenig Punkte?
  if (GetAmmoPoints() < points)
    return PlayerMessage(GetOwner(Contained()),"$NeededPoints$", Contained(), points);

  //Custom-Ammo erstellen und füllen
  var ammo = CreateContents(CUAM, Contained());
  ammo->SetAmmoID(idAmmo);
  ammo->SetAmmoCount(count, true);
  DoAmmoPoints(-points);
  Sound("PackAmmo.ogg",0,0,0,GetOwner(Contained()));

  //Hinwechseln
  ShiftContents(Contained(),0,CUAM);
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
   if(ContentsCount(GetID(),pObj))
    return true;
  }
  return false;
}

public func ControlThrow(object pClonk)
{
  return true;
}

/* Munitionspunkte */

public func GetAmmoPoints()
{
  return ammopoints;
}

public func DoAmmoPoints(int iChange)
{
  ammopoints = BoundBy(ammopoints+iChange,0,MaxAmmoPoints());

  return ammopoints;
}

/* Regenerierungseffekt */

public func FxAMPKRegenerateTimer(pTarget, iEffectNumber, iEffectTime)
{
  if(!Contained()) return 1;
  if(GetAmmoPoints() < MaxAmmoPoints())
   DoAmmoPoints(1);

  return 1;
}

/* Lämpcheneffekt */

public func FxAMPKLightTimer(pTarget, iNo, iTime)
{
  if(GetAmmoPoints() < 20) return 1;
  if(!Contained())
   CreateParticle("FapLight", 1, -2, 0, 0, 5*5, RGBa(BoundBy(InvertA1(255*GetAmmoPoints()/300,255)+10,0,255), 255*GetAmmoPoints()/300),this);
  if(Contents(0,Contained()) == this)
   if(WildcardMatch(GetAction(Contained()), "*Armed*"))
    CreateParticle("FapLight", (GetDir(Contained())*1), -2, 0, 0, 5*5, RGBa(BoundBy(InvertA1(255*GetAmmoPoints()/300,255)+10,0,255), 255*GetAmmoPoints()/300),this);
}

/* Gruppenaufstockung */

public func FxAMPKRestockingTimer(pTarget, iEffectNumber, iEffectTime)
{
  //Kriterien
  if(!Contained())			return 1;	//Nicht im Freien
  if(Contained(Contained()))		return 1;	//Container nicht im Freien
  if(GetAmmoPoints() < 30)		return 1;	//Nur wenn noch Punkte da sind
  if(Contents(0,Contained()) != this)	return 1;	//Nur, falls angewählt
  if(GetID(Contained()) == FKDT)	return 1;	//Im FakeDeath-Objekt?

  //Harte Vorauswahl überlebt? Los geht's.
  for(var target in FindObjects(Find_OCF(OCF_Alive),			//Ziel am Leben?
                                Find_Distance(80),			//In Reichweite?
                                Find_NoContainer(),			//Im Freien?
                                Find_Allied(GetOwner(Contained())),	//Verbündet?
                                Find_Exclude(Contained())))  		//Nicht der selbe Clonk?
  {

    if(!(target->~IsClonk()))						//Ziel ein Clonk?
     continue;

    //Munitionsbedarf feststellen
    var highestammo = 0, ammoID = 0;
    for(var i = 0; i < ContentsCount(0,target); i++)
     if(Contents(i,target)->~IsWeapon())
      for(var j = 0; j < Contents(i,target)->GetFMCount(); j++)
      {
       var ammocount, weapon = Contents(i,target);
       if(weapon->GetFMData(FM_AmmoLoad,j) <= 3)
        ammocount = weapon->GetFMData(FM_AmmoLoad,j)*10;
       else
        ammocount = weapon->GetFMData(FM_AmmoLoad,j)*3;
       if(GetAmmo(weapon->GetFMData(FM_AmmoID,j),target) < ammocount)
       {
        if(!ammoID)
         ammoID = weapon->GetFMData(FM_AmmoID,j);
        if(highestammo < ammocount)
         highestammo = ammocount;
       }
      }

    if(!ammoID)
     break;

    //Munition hinzufügen
    var factor;
    if(ammoID == STAM)  //Normale Kugeln = 1 Punkt
     factor = 1;
    if(ammoID == GRAM)  //Granaten = 5 Punkte
     factor = 5;
    if(ammoID == MIAM)  //Raketen = 10 Punkte
     factor = 10;
    if(!factor)         //Alles andere = 2 Punkte
     factor = 2;
    if(ammoID->MaxAmmo()/10*factor > GetAmmoPoints() || GetAmmo(ammoID,target) >= highestammo)
     break;

    PlayerMessage(GetOwner(Contained()),"$AmmoRecieved$", target, ammoID->MaxAmmo()/10, ammoID);
    PlayerMessage(GetOwner(target),"$AmmoRecieved$", target, ammoID->MaxAmmo()/10, ammoID);
    DoAmmo(ammoID, ammoID->MaxAmmo()/10, target);
    Sound("Resupply.ogg");
    DoAmmoPoints(-ammoID->MaxAmmo()/10*factor);

    //Punkte bei Belohnungssystem
    DoPlayerPoints(BonusPoints("Restocking",ammoID->MaxAmmo()/10*factor), RWDS_TeamPoints, GetOwner(Contained()), Contained(), IC14);
  }
}

/* Infos */

protected func CalcValue()
{
  //Wert errechnen
  return ammopoints*(GetValue(0,GetID())/(StartAmmoPoints()/MaxAmmoPoints()))/MaxAmmoPoints();
}

/* Ammo-HUD */

func CustomHUD(){return true;}

func UpdateHUD(object pHUD)
{
  pHUD->Charge(ammopoints,MaxAmmoPoints());
  pHUD->Ammo(ammopoints, MaxAmmoPoints(), GetName(), true);
}

/* Sounds */

protected func Hit()
{
  Sound("AmmoBoxHit*.ogg");
  return 1;
}

protected func Selection()
{
  Sound("FAPK_Charge.ogg");
  return 1;
}
