#pragma once
#include <wx/wx.h>
#include "Server.h"

class ServerFrame : public wxFrame
{
public:
	ServerFrame(const wxString& title, Server* server);
	wxPanel* GetPanel();
	wxStaticText* AddText(wxPanel* panel, std::string inText);
	Server* mServer;
	
private:
	void StartPressed(wxCommandEvent& evt);
	void StopPressed(wxCommandEvent& evt);
	wxPanel* bottomPanel;

	wxDECLARE_EVENT_TABLE();
};
