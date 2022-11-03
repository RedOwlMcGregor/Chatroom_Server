#include "App.h"
#include "ServerFrame.h"
#include "Server.h"
#include "wx/wx.h"

bool App::OnInit()
{
	Server* server = new Server();
	ServerFrame* serverFrame = new ServerFrame("Server Window", server);
	return true;
}



wxIMPLEMENT_APP(App);