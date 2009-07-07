/*-- Fass --*/

#strict

// Anzahl Materialpixel, die in ein Fass passen
static const BARL_MaxFill = 200;

// F�llstand
local iFillLevel;

// F�llmaterial+1
// Benutzt nur vom Lavafass, um zwischen Lava und DuroLava zu unterscheiden
// Kann f�r F�sser aus alten Spielst�nden 0 sein!
local iFillMaterial;

// Gegenw�rtiges Material, mit dem nicht aufgef�llt werden konnte
local iBackMaterial;

// ID des leeren Fasses
global func EmptyBarrelID() { return(BARL); }

// Verkauf
protected func SellTo()
{
  // Fasskonfiguration: Gegebenenfalls Verkauf zum gef�llten Fass
  if (BarrelConfiguration() & 2) return(GetID());
  // Ansonsten beim Verkauf entleeren
  return(EmptyBarrelID());
}

/* TimerCall */

private func FillCheck()
{
  // Material an aktueller Position �berpr�fen
  var iMaterial;
  if ( ((iMaterial = GetMaterial()) == -1) || (iMaterial == iBackMaterial) ) return(0);
  // Fass f�r dieses Material suchen
  var idBarrel;
  if (!(idBarrel = GetBarrelType(iMaterial))) return(iBackMaterial = iMaterial);
  // Fass f�llen wenn entweder leer oder mit dem gleichen Material aufgef�llt werden kann
  if(BarrelMaterial() != -1 && BarrelMaterial() != iMaterial) return(0);
  // Wenn nicht bereits voll
  if(iFillLevel >= BarrelMaxFill()) return(0);
  // Fass f�llen
  iFillLevel += ExtractMaterialAmount(0, 0, iMaterial, BarrelMaxFill() - iFillLevel);
  // Zum vollen Fass werden, wenn wir es nicht bereits sind
  if(BarrelMaterial() == -1)
  {
    iFillMaterial = iMaterial+1;
    ChangeDef(idBarrel);
  }
  return(1);
}

/* F�llung */

// Aktuelle F�llung
public func GetAmount() { return(iFillLevel); }

// F�llmaterial
public func BarrelMaterial() { return(-1); }
public func BarrelMaterialName() { return(); }

// Maximale F�llung
public func BarrelMaxFill() { return(BARL_MaxFill); }

// F�llung erh�hen/verringern
public func BarrelDoFill(int iChange, int iMat)
{
  var iNewFill = BoundBy(iFillLevel + iChange, 0, BarrelMaxFill());
  if (iNewFill == iFillLevel) return();
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  // Aktuelles F�llmaterial ggf. �berschreiben
  if (iMat && iChange>0) iFillMaterial = iMat;
  // Tats�chliche �nderung des F�llstandes zur�ckgeben
  return(iNewFill);
}

// Fass voll?
public func BarrelIsFull() 
{ 
  return(iFillLevel == BarrelMaxFill());
}

// Ein Pixel F�llung entfernen
public func BarrelExtractLiquid()
{
  // F�llung verringern
  if (!BarrelDoFill(-1)) 
    // Konnte F�llung nicht verringern: Fass ist leer/kein Material
    return(-1);
  // F�llmaterial speichern    
  var iMaterial = BarrelMaterial();
  // Jetzt leer?
  if (!iFillLevel) 
    ChangeDef(EmptyBarrelID());
  // F�llmaterial zur�ckliefern
  return(iMaterial);
}

/* Wert nach F�llung */

// Wert = Wert(Leeres Fass) + (Wert(Volles Fass) - Wert(Leeres Fass)) * (F�llung / MaxF�llung)
protected func CalcValue()
{
  var iValueEmpty = GetValue(0, EmptyBarrelID());
  var iValueFull = GetValue(0, GetID());
  return(iValueEmpty + ((iValueFull - iValueEmpty) * iFillLevel) / BarrelMaxFill());
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("BarrelImpact*.ogg");
  BarrelEject();
  return(1);
}

private func BarrelEject()
{
  var strMaterial;
  if (!(strMaterial = BarrelMaterialName())) return(1);
  Sound("Splash1");
  CastPXS(strMaterial, iFillLevel, 20);
  iFillLevel = 0;
  ChangeDef(EmptyBarrelID());
  return(1);  
}

/* Initialisierung */

protected func Initialize()
{
  // Kein Material im Hintergrund
  iBackMaterial = -1;
  // Materialfass? F�llen
  if (BarrelMaterialName()) 
    iFillLevel = BarrelMaxFill();
  return(1);
}


/* Aktivierung auf Doppel-Graben */

protected func Activate()
{
  [$TxtEject$]
  // Ausleeren
  return(BarrelEject());
}
  
/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
