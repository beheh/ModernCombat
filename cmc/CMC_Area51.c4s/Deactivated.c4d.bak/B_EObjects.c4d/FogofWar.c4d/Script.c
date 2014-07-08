/*-- Kriegsnebel --*/

#strict 2

static FoW_VarDebug;


/* Initialisierung */

protected func Initialize()
{
  FoW_VarDebug = false;
}

/* Szenariofunktionen */

//Gibt an, ob Clonks sehen k�nnen, was hinter ihrem R�cken vorgeht (R�ckgabe: true/false)
global func FoW_CanSeeBehindBack()
{
  return(false);
}

//Gibt an, ob nur der ausgew�hlte Clonk des Spielers eine Sicht hat (R�ckgabe: true/false)
global func FoW_OnlyCursor()
{
  return(true);
}

//Gibt an, ob Clonks aus Geb�uden sehen k�nnen (R�ckgabe: true/false)
global func FoW_CanSeeOutOfBuildings()
{
  return(true);
}

//Gibt an, ob die Sichthelfer mit Debugdaten gezeichnet werden sollen (R�ckgabe: true/false)
global func FoW_Debug()
{
  return(FoW_VarDebug);
}

//Gibt an, ab welcher unteren Pixelschwelle die Berechnung abgebrochen werden soll (Minimum: 0)
global func FoW_MinimumRange()
{
  return(0);
}

//Gibt an, mit welcher Genauigkeit die Mitte des Raums bestimmt wird (Minimum: 1)
global func FoW_CalcResolution()
{
  return(1);
}

//Gibt an, mit welcher Genauigkeit die Mitte des Raums bestimmt wird (Theoretisches Minimum ist 1, wird jedoch absolut unspielbar; bei R�ckgabewert false wird immer der Radius des letzten Punktes verwendet)
global func FoW_SightResolution()
{
  return(100);
}

//Gibt an, wieviele Pixel die Sicht in den nicht einsehbaren Bereich (Erdreich) reicht (0 hei�t dass genau bis zum Hindernis sichtbar ist, ansonsten Entfernung in Pixeln)
global func FoW_Tolerance()
{
  return(100);
}

/* Aufruf in den Clonks */

public func SwitchDebug()
{
  for (var pClonk in FindObjects(Find_Func("ResetView")));
  return(FoW_VarDebug = !FoW_VarDebug);
}

private func Execute()
{
  for (var pClonk in FindObjects(Find_Func("SetRealisticView")))
  {
    pClonk->~SetRealisticView();
  }
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}