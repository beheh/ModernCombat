/*-- Use Skins --*/

#strict

/* Einstellungen */

func SkinCount(){return(0);}


/* Allgemeines */

func SetupSkin()
{
  if(!SkinCount()) return(false);

  var skin = GetSkin();
  if(!skin) skin = Random(SkinCount()+2);
  SetSkin(skin);
  
  return(true);
}

func GetSkin()
{
  return(GetCrewExtraData(this(),"Skin"));
}

func SetSkin(int iSkin)
{
  SetCrewExtraData(this(),"Skin",iSkin);

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