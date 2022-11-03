#include "ServerFrame.h"
#include "Server.h"
#include <wx/wx.h>

enum IDS // This is used to give customID's 
{
	STARTB_ID = 2, //ID for the start button
	ENDB_ID = 3 // ID for the stop button
};

wxBEGIN_EVENT_TABLE(ServerFrame, wxFrame) //initiates a event table to add event handling
EVT_BUTTON(STARTB_ID, ServerFrame::StartPressed) //binds the ButtonID to the StartPressed Function
EVT_BUTTON(ENDB_ID, ServerFrame::StopPressed) //binds the ButtonID to the StopPressed Function
wxEND_EVENT_TABLE();

ServerFrame::ServerFrame(const wxString& title, Server* server) : wxFrame(nullptr, wxID_ANY, title) // inherits from the wxFrame class giving us access to all its functions. wxString is just the title of the window.
{
	
	SetMinClientSize(wxSize(800, 600));
	SetMaxClientSize(wxSize(800, 600));

	this->mServer = server;
	server->AssignFrame(this);

	Show();

	bottomPanel = new wxPanel(this, wxID_ANY, wxPoint(250, 25), wxSize(300, 400)); // This panel allows controls such as buttons to be attachable to the window.

	wxButton* startButton = new wxButton(this, STARTB_ID, "Start Server", wxPoint(100, 500), wxSize(200, 35));

	wxButton* stopButton = new wxButton(this, ENDB_ID, "Stop Server", wxPoint(500, 500), wxSize(200, 35));

	CreateStatusBar();
}

wxStaticText* ServerFrame::AddText(wxPanel* panel, std::string inText)
{
	wxStaticText* text;

	if (panel->GetChildren().GetCount() == 0)
	{
		text = new wxStaticText(panel, wxID_ANY, inText);
		return text;
	}

	wxWindowList::value_type recentChild = panel->GetChildren().back();

	text = new wxStaticText(panel, wxID_ANY, inText, wxPoint(0, recentChild->GetPosition().y + recentChild->GetSize().y));

	return text;
}

void ServerFrame::StartPressed(wxCommandEvent& evt)
{
	bottomPanel->DestroyChildren();

	wxLogStatus("Starting Server");
	AddText(bottomPanel, "Starting Server... \n");
	AddText(bottomPanel, mServer->Start());

}

void ServerFrame::StopPressed(wxCommandEvent& evt)
{
	bottomPanel->DestroyChildren();

	wxLogStatus("Stopping Server");
	AddText(bottomPanel, "Stopping Server... \n");
	AddText(bottomPanel, mServer->Stop());
}

wxPanel* ServerFrame::GetPanel()
{
	return bottomPanel;
}




