/*-- Neues Script --*/

#strict 2

#appendto CLNK

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
 
  //In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if(Contained())
  {
    if(Contained()->~ContainedSpecial2(this))
      return 1;
    if((GetCategory(Contained()) & C4D_Structure) || (GetCategory(Contained()) & C4D_Vehicle))
    {
      ExecuteCommand();
      return SetCommand(this, "Context", 0 ,0, 0, Contained());
    }
  }
  //IsRiding: Für die Waffenplattform (und mögliche folgende Dinge); im Moment hauptsächlich, um das Kontextmenü des Inventars zu benutzen
  //Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if(GetAction() == "Push" || IsRiding())
  {
    ExecuteCommand();
    return SetCommand(this, "Context", 0, 0, 0, GetActionTarget());
  }
  //Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if(Contents(0))
  {
    ExecuteCommand();
    return SetCommand(this, "Context", 0, 0, 0, Contents(0));
  }
  //Ansonsten das Kontextmenü des Clonks öffnen
  ExecuteCommand();
  return SetCommand(this, "Context", 0, 0, 0, this);
}
