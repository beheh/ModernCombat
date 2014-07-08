/*-- Hai --*/

//Haie meiden Schlauchboote und verfügen über erweiterte Angriffe, zudem sind ihre Leichen nicht mehr anfassbar.

#strict 2

#appendto SHRK

public func IsBulletTarget()	{return true;}
public func SearchRange()	{return 500;}
public func NoSentryGunTarget()	{return true;}


private func SwimStart() 
{
  //Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this)) return;

  //Beute am Maul beißen
  if (Attack()) return;

  //Beute in Entfernung suchen
  var pPrey;
  if (pPrey = FindPrey())
  {
    //Immer von hinten mit einem Abstand angreifen
    var iFromSide = +1; if (GetDir(pPrey)) iFromSide = -1;
    SetCommand(this, "MoveTo", 0, GetX(pPrey) + 10 * iFromSide, GetY(pPrey));
  }

  //Booten aus dem Weg gehen
  var boot;
  if(boot = FindObject2(Find_ID(SPBT),Find_Distance(200)))
    if(GetY(boot) - GetY() < 40)
      SetComDir(BoundBy(GetComDir(), COMD_DownRight, COMD_DownLeft));
}

private func FindPrey()
{
  var prey;
  // Zuerst schwimmende Beute suchen...
  prey = FindObject(0, -SearchRange(), -100, SearchRange()*2, 200, OCF_Prey, "Swim", 0, NoContainer());
  // ...dann auch andere Beute im Wasser suchen (z.B. stehende Aquaclonks)
  if (!prey)
  {
    while (prey = FindObject(0, -SearchRange(), -100, SearchRange()*2, 200, OCF_Prey, 0, 0, NoContainer(), prey))
      if (GetAlive(prey))
        if (InLiquid(prey))
          break;
  }
  //Gefundene Beute zurückliefern
  return prey;  
}

private func Attack()
{
  if (GetEffect("IntAttackDelay", this) || GetAction() != "Swim") return;

  // Beute suchen
  var pPrey = FindObject(0, -20 + 40 * GetDir(), 0, 0, 0, OCF_Prey, 0, 0, NoContainer());
  if (!pPrey || !GetAlive(pPrey)) return;

  //Schaden
  DoDmg(10+Random(20),DMG_Melee,pPrey,0,0,GetID());
  //Zufällig das Barett eines Sanitäter-Opfers "stehlen"
  if(GetID(pPrey) == MDIC && !Random(6) && (!GetAlive(pPrey) || pPrey->~IsFakeDeath()))
    SetGraphics(0, this, SKSN);
  SetAction("Jaw");
  Sound("Munch*");
  AddEffect("IntAttackDelay", this, 1, 35);
  return 1;
}

protected func Activity() 
{
  _inherited();

  //Booten aus dem Weg gehen
  var boot;
  if(boot = FindObject2(Find_ID(SPBT),Find_Distance(200)))
    if(GetY(boot) - GetY() < 40)
      SetComDir(BoundBy(GetComDir(), COMD_DownRight, COMD_DownLeft));
}

/* Tod */

protected func Death()
{
  SetDir(0);
  ChangeDef(DSHK);
  SetAction("Dead", this);
  SetObjectLayer(this);
  return 1;
}