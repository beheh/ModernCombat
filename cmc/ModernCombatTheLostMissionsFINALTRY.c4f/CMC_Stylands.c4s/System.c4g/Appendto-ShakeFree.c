/*-- ShakeFree --*/

//Erweiterte Killverfolgung für Erdbeben.

#strict 2


global func ShakeFree(int x,int y, int str)
{
  //Wenn ein Controller bekannt ist: Kontakt aller Objekte vor dem Treffer speichern
  var objs, contacts, num, ctrl = GetController();
  if(ctrl != NO_OWNER)
  {
    objs = FindObjects(Find_InRect(x-GetX()-str-50, y-GetY()-str-50, str*2+100, str*2+100));
    if(num = GetLength(objs))
    {
      contacts = CreateArray(num);
      for(var i=0; i<num; ++i)
        contacts[i] = GetContact(objs[i], -1, -1);
    }
  }

  if(!inherited(x,y,str, ...))
    return false;

  //Für alle Objekte, die danach weniger Kontaktpunkte haben, Controller/Killer setzen
  for(var i=0; i<num; ++i)
    if(GetContact(objs[i], -1, -1) < contacts[i])
      if(GetAlive(objs[i]))
        //Killer verzögert setzen, da Jump-Aktivität gegebenenfalls verspätet kommt
        objs[i]->Schedule(Format("SetKiller(%d)", ctrl), 2, 1);
      else
        SetController(ctrl, objs[i]);

  return true;
}