/*-- Verbandsbox --*/

#strict 2

func HealRate()			{return 3;}
func HealAmount()		{return 10;}
public func NoArenaRemove()	{return true;}


/* Aufnahme */

public func RejectEntrance(object pObj)
{
  var pFAP;

  //Clonk heilen wenn nötig, sonst in dessen EHP einfügen sofern vorhanden
  if(pObj->GetAlive() && (GetEnergy(pObj) < GetPhysical("Energy",0,pObj)/1000))
  {
    if(!GetEffect("*Heal*",pObj))
    {
      AddEffect("DragninHeal",pObj,20,1,0,DGNN,HealAmount(),HealRate());
      Sound("FAPK_Healing*.ogg");
      Sound("PackGrab*.ogg");

      RemoveObject();
    }
  }
  else if((pFAP = FindContents(FAPK, pObj)) && pFAP->~GetPackPoints() < pFAP->~MaxPoints() && !GetEffect("FAPHeal", pFAP)) //Falls Clonk voll geheilt ist und ein EHP besitzt, dieses auffüllen.
  {
    pFAP->~DoPackPoints(HealAmount());
    PlayerMessage(GetOwner(pObj), "$Refilled$", pObj, FAPK, HealAmount());
    Sound("Merge.ogg",0,pFAP,0,GetOwner(pObj)+1);
    Sound("PackGrab*.ogg");

    RemoveObject();
  }

  return true;
}

/* Schaden */

protected func Damage(int iChange)
{
  if(GetDamage() < 10) return;
  Destruct();
}

protected func Destruct()
{
  //Effekte
  CastParticles("Paper", RandomX(4, 8), 40, 0, 0, 20, 35, RGB(180, 180, 180), RGBa(240, 240, 240, 150));
  Sound("FAPK_Hit*.ogg", false, this);

  //Verschwinden
  RemoveObject();
}

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Aufschlag */

protected func Hit()
{
  Sound("FAPK_Hit*.ogg");
}