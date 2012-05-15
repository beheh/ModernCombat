/*-- Neues Script --*/

#strict 2

#appendto CLNK

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
 
  //In einem Geb�ude oder Fahrzeug: das Kontextmen� des Geb�udes �ffnen
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
  //IsRiding: F�r die Waffenplattform (und m�gliche folgende Dinge); im Moment haupts�chlich, um das Kontextmen� des Inventars zu benutzen
  //Fasst ein Objekt an: Kontextmen� des angefassten Objekts �ffnen
  if(GetAction() == "Push" || IsRiding())
  {
    ExecuteCommand();
    return SetCommand(this, "Context", 0, 0, 0, GetActionTarget());
  }
  //Tr�gt ein Objekt: Kontextmen� des ersten getragenen Objekts �ffnen
  if(Contents(0))
  {
    ExecuteCommand();
    return SetCommand(this, "Context", 0, 0, 0, Contents(0));
  }
  //Ansonsten das Kontextmen� des Clonks �ffnen
  ExecuteCommand();
  return SetCommand(this, "Context", 0, 0, 0, this);
}
