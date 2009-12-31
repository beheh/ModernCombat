/*-- Gepackter Munitionsclip --*/

#strict 2

local ammoid,count;

public func NoWeaponChoice(){return true;}//LOLZ!
public func IsAmmoPacket() {return true;}//:S
public func AmmoID(){return ammoid;}
public func AmmoCount(){return count;}

public func SetAmmoID(id idType)
{
  if(idType->~IsAmmo())
  {
    ammoid = idType;
    SetGraphics(0,0,idType,1,GFXOV_MODE_Picture);
    SetObjDrawTransform(500, 0, 0, 0, 500, 0, this, 1);
  }
  return false;
}

public func GetMax()
{
  var max = ammoid->~MaxAmmo()/10;
  if(!max) max = 50;
  return max;
}

public func SetAmmoCount(int iCount)
{
  if(!iCount)
  {
    RemoveObject();
    return 0;
  }
  count = BoundBy(iCount,0,GetMax());
  return count;
}

public func DoAmmoCount(int iCount)
{
  var new = BoundBy(iCount,0,GetMax());
  var dif = new-AmmoCount();
  count   = new;
  return dif;
}

//Von STAM kopiert.
private func OnTransfer(){ Sound("Resupply.ogg"); }

protected func Activate(object pObj) {		// Doppelgraben
  return TransferAmmo(pObj);
}

public func MayTransfer(object pObj)
{
  if(!pObj) return false;
  var MaxAmmo = AmmoID()->~MaxAmmo();
  if(MaxAmmo)
    if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
      return false;
  return true;
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
  if(!pObj) return false;
  if(NoAmmo()) return false;
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj))
  {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return ;
  }

  // Nachricht ausgeben
  PlayerMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCount(),AmmoID());//Das ist mal keine Hilfenachricht, weil niemand wissen kann wieviel da drin ist. ;D
  
  var old = count;
  SetAmmoCount(old-DoAmmo(AmmoID(),AmmoCount(),pObj));
  
  pObj->~AmmoTransferred();
  
  if(!OnTransfer()) RemoveObject();

  return true;
}

protected func Hit()
{
  Sound("Ammobaghit*.ogg");
  return 1;
}
