/*-- Zusatzfunktionen --*/

//Weitere Funktionen.

#strict 2


global func InvertA1(int Val1, int Val2)
{
  return Val2/2 + (Val2/2 -Val1);
}

/* Prüft, ob ein Objekt ausschließlich in-solidem Material ist */

global func StuckSolid(object pTarget) 
{ 
  if(!pTarget && !(pTarget = this)) 
    return true;
 
  for(var i = 0, x, y; i <= GetVertexNum(); i++) 
  { 
    x = GetVertex(i, false, pTarget); y = GetVertex(i, true, pTarget); 
    if(!GetMaterialVal("DigFree", 0, GetMaterial(x, y)) && GBackSolid(x, y)) 
      return true; 
  } 
}

/* Objekt zu Overlay (Sven2) */

global func Object2Overlay(object target, object obj)
{
  //Sicherheit/Lokaler Aufruf
  if(!target)			return 0;
  if(!obj) if (!(obj = this))	return 0;

  //Neues Overlay mit Parametern des Objektes erzeugen
  var overlay_mode = GFXOV_MODE_Base;
  var overlay_action = 0;
  var overlay_id = GetUnusedOverlayID(GFX_Overlay, target);
  if(!ActIdle(obj))
    {overlay_mode = GFXOV_MODE_Action; overlay_action = GetAction(obj);}
  if(!SetGraphics(0, target, GetID(obj), overlay_id, overlay_mode, overlay_action, GetObjectBlitMode(obj), 0))
    return 0;
  if(GetClrModulation(obj))
    SetClrModulation(GetClrModulation(obj), target, overlay_id);
  SetObjDrawTransform(1000,0,(GetX(obj)-GetX(target))*1000, 0,1000,(GetY(obj)-GetY(target))*1000, target, overlay_id);

  //Altobjekt entfernen
  RemoveObject(obj);

  //Erzeugtes Overlay zurückgeben
  return overlay_id;
}
