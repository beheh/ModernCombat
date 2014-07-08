/*-- Glasbox --*/

#strict 2


/* Aufnahme */

public func Entrance(object pObj)
{
  Open(pObj);
}

/* Öffnen */

public func Activate(object pObj)
{
  Open(pObj);
}

/* Gegenstand erstellen */

public func Open(object pObj)
{
  //Nur öffnen, wenn der Träger ein spielergesteuertes Objekt ist
  if((GetOCF(pObj) & OCF_CrewMember))
  {
    //Zufälligen Gegenstand erstellen
    var rnd=Random(12);
    if(!rnd--) CreateContents(FGRN, pObj);	//Feldgranate
    if(!rnd--) CreateContents(FRAG, pObj);	//Splittergranate
    if(!rnd--) CreateContents(PGRN, pObj);	//Phosphorgranate
    if(!rnd--) CreateContents(SGRN, pObj);	//Rauchgranate
    if(!rnd--) CreateContents(STUN, pObj);	//Blendgranate
    if(!rnd--) CreateContents(SRBL, pObj);	//Sensorball

    if(!rnd--) CreateContents(BTBG, pObj);	//Sprengfallentasche
    if(!rnd--) CreateContents(CDBT, pObj);	//Defibrillator
    if(!rnd--) CreateContents(DGNN, pObj);	//Dragnin

    if(!rnd--) {var wpn = CreateContents(RVLR, pObj); wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));}	//Revolver
    if(!rnd--) {var wpn = CreateContents(ATWN, pObj); wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));}	//APW
    if(!rnd--) {var wpn = CreateContents(PPGN, pObj); wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));}	//Schrotflinte
    if(!rnd--) {var wpn = CreateContents(MNGN, pObj); wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));}	//Maschinengewehr

    //Effekte
    Sound("GlassBreak*.ogg");
    CastParticles("Glas", 10, 50, 0,0, 20,40, RGBa(200,200,200), RGBa(200,200,200));

    //Verschwinden
    RemoveObject();
  }
}

/* Aufschlag */

public func Hit()
{
  Sound("GlassBreak*.ogg");
}