/*-- Use Skins --*/

#strict 2

/* Einstellungen */

public func SkinCount() { return 0; }


/* Allgemeines */

public func SetupSkin()
{
  if(!SkinCount()) return false;

  var skin = GetSkin();
  if(!skin) skin = RandomX(1,SkinCount()+1);
  SetSkin(skin);
  
  return true;
}

public func GetSkin()
{
  return GetCrewExtraData(this,"Skin");
}

public func SetSkin(int iSkin)
{
  SetCrewExtraData(this,"Skin",iSkin);

  if(iSkin > 1)
    SetGraphics(Format("Skin%d",iSkin));
  else
    SetGraphics();
}


/* Overloads */

protected func Initialize()
{
  SetupSkin();
  
  return(_inherited(...));
}