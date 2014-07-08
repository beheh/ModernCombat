/*-- Funken --*/

#strict 2

protected func Hit()		{return(SetAction("Sparkle"));}
protected func Removal()	{return(RemoveObject());}


protected func Initialize()
{
  return(SetAction("Fall"));
}

/* Funken erstellen */

protected func CreateSpark()
{
  //Immer je links und rechts einen Spark erschaffen, die sich an der Mittelachse spiegeln
  var x = Random(LandscapeWidth() / 2);
  CreateObject(GetID(), x+25-GetX(),Random(LandscapeWidth()-20)-20, -1);
  CreateObject(GetID(), LandscapeWidth()-x+25-GetX(),Random(LandscapeWidth()-20)-20, -1);

  return true;
}

protected func CheckHit()
{
  if(Stuck() || GetContact(0, -1, CNAT_Bottom)) return(Hit());
}

protected func CastObject()
{
  CreateObject(RndObjectID(), 0,0, -1);
  Sound("Boing");
  return true;
}

protected func RndObjectID()
{
  var r = Random(70);
  if(!r) return(ASTR);
  if(!--r) return(PPGN);
  if(!--r) return(MNGN);
  if(!--r) return(RTLR);
  if(!--r) return(SGST);
  if(!--r) return(SMGN);
  if(!--r) return(ATWN);

  var r = Random(50);
  if(!r)   return(CATA);
  if(!--r) return(MAGW);
  if(!--r) return(TRE1);
  if(!--r) return(WMIL);
  if(!--r) return(SNKE);
  if(!--r) return(BALN);
  if(!--r) return(BLMP);
  if(!--r) return(LOAM);
  if(!--r) return(METL);
  if(!--r) return(JTPK);
  if(!--r) return(FGRN);
  if(!--r) return(FRAG);
  if(!--r) return(SGRN);
  if(!--r) return(XBRL);
  if(!--r) return(GSBL);
  if(!--r) return(PBRL);
  if(!--r) return(HBRL);
  if(!--r) return(TBRL);

  r = Random(8);
  if(!r)   return(FLNT);
 if(!--r) return(SFLN);
 if(!--r) return(TFLN);
 if(!--r) return(STFN);
 if(!--r) return(FBMP);
 if(!--r) return(CDBT);

  return(FAPK);
}