/*-- Flie�band --*/

#strict 2

local parts;

public func IgnoreProductionLine()	{return true;}


/* Initialisierung */

func Initialize()
{
  //Voreinstellung
  Set(2);
  return 1;
}

public func SetSpeed(int iSpeed)
{
  for(var part in parts)
    part->SetSpeed(iSpeed);

  return true;
}

public func ChangeAction()
{
  var newAct, act = GetActions();
  if(act == "Left")
    newAct = "Right";
  else
    newAct = "Left";

  SetActions(newAct);
}

/* Mit dieser Funktion wird ein neues Laufband erzeugt. a steht f�r die Anzahl der Teile */

public func Set(a)
{
  CreateProductionLine(a);
}

/* Funktion f�r das Setzen der Aktivit�t aller Teile */

public func SetActions(string s)
{
  for(var part in parts)
  {
    var i = GetPhase(part);
    part->SetAction(Format("%s", s));
    part->SetPhase(i);
  }
}

/* Abfrage der Aktivit�t aller Teile */

private func GetActions()
{
  for(var part in parts)
    var ans = GetAction(part);

  return ans;
}

/* Flie�band erstellen */

func CreateProductionLine(a)
{
  //Bei 1 und darunter sowie bei �ber 30 passiert nichts.
  if((a <= 1) || (a > 30)) return;
 
  //Alte Teile l�schen:
  if(parts) ClearParts();
  parts = [];
  
  //Mittelst�cke platzieren:
  if(!(a == 2))
    for(var i = ((a-2)/(-2))*40+GetBla(a); i <= ((a-2)/2)*40-GetBla(a); i += 40)
      parts[GetLength(parts)] = CreateObject(PM1C, i, 10, -1);

  //Endst�cke platzieren:
  parts[GetLength(parts)] = CreateObject(PL1C, (a/(-2))*40+GetBla(a), 10, -1);
  parts[GetLength(parts)] = CreateObject(PR1C, (a/2)*40-GetBla(a), 10, -1);
  
  var checkBtm = !GBackSemiSolid(0, 15);
  
  //Allen Teilen Erschaffer �bergeben und erneut initializieren:
  for(var part in parts)
  {
    LocalN("father", part) = this;
    LocalN("fCheckBottom", part) = checkBtm;
    part->Initialize();
  }
  
  SetShape((-a*40)/2+8, -10, a*40-16, 20);
}

/* Zerst�rung */

func Destruction()
{
  ClearParts();
}

func ClearParts()
{
  for(var part in parts)
    RemoveObject(part);
}

/* Steuerung */

public func ConsoleControl(int i)
{
  if(i == 1) return "$Left$";
  if(i == 2) return "$Right$";
  if(i == 3) return "$Stop$";
}

public func ConsoleControlled(int i)
{
  if(i == 1 && GetActions() != "Left") SetActions("Left");
  if(i == 2 && GetActions() != "Right") SetActions("Right");
  if(i == 3 && GetActions() != "Stop") SetActions("Stop");
}

private func GetBla(a)
{
  if(a==2||a==4||a==6||a==8||a==10||a==12||a==14||a==14||a==16||a==18||a==20||a==22||a==24||a==26||a==28||a==30)
    return 20;
  else return;
}
