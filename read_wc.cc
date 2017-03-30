#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <stdio.h>     
#include <stdlib.h>    
#include "read_wc.h"

void read_wc(bool verbose=true)
{
    // Load the library with class dictionary info
    // (create with "gmake shared")
    char* wcsimdirenv;
    wcsimdirenv = getenv ("WCSIMDIR");

    /*if(wcsimdirenv !=  NULL){
     gSystem->Load("${WCSIMDIR}/libWCSimRoot.so");
     }else{
     gSystem->Load("../libWCSimRoot.so");
     }*/
  
    gSystem->Load("libWCSimRoot.so");

    TFile *file;
    // Open the file
    filename = "wcsim_100_e-_new_flat.root";
    electron = true;
    
    /*if (filename==NULL){
     file = new TFile("../wcsim.root","read");
     }else{
     file = new TFile(filename,"read");
     }*/
    file = new TFile(filename,"read");

    if (!file->IsOpen()){
        cout << "Error, could not open input file: " << filename << endl;
        return -1;
    }
  
    // Get the pointer to the tree from the file

    TTree *wcsimT = (TTree*)file->Get("wcsimT");
  
    // Create a WCSimRootEvent to put stuff from the tree in

    WCSimRootEvent* wcsimrootsuperevent = new WCSimRootEvent();

    // Set the branch address for reading from the tree
    // tree has branch wcsimrootevent

    TBranch *branch = wcsimT->GetBranch("wcsimrootevent");
    branch->SetAddress(&wcsimrootsuperevent);
  
    // Alternatively
    // tree->SetBranchAddress("wcsimrootevent",&wcsimrootevent);
    
    // Force deletion to prevent memory leak
    wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);

    // geotree has 1 entry containing the geometry information for simulated detector
    // wcsimGeoT has branch wcsimrootgeom

    TTree *geotree = (TTree*)file->Get("wcsimGeoT");
    WCSimRootGeom *geo = 0;

    //WCSimRootGeom *geo = new WCSimRootGeom()
    geotree->SetBranchAddress("wcsimrootgeom", &geo);
    
    if(geotree->GetEntries() == 1) printf("Confirmed... GeoTree has 1 entry. \n \n");
    if (geotree->GetEntries() == 0) exit(9);

    geotree->GetEntry(0);

    // start with the main "subevent", as it contains most of the info
    // and always exists.
  
    WCSimRootTrigger* wcsimrootevent;

    //TH1F *h1 = new TH1F("PMT Hits", "PMT Hits", 8000, 0, 8000);
    //TH1F *hvtx0 = new TH1F("Event VTX0", "Event VTX0", 200, -1500, 1500);
    //TH1F *hvtx1 = new TH1F("Event VTX1", "Event VTX1", 200, -1500, 1500);
    //TH1F *hvtx2 = new TH1F("Event VTX2", "Event VTX2", 200, -1500, 1500);
  
    int nevent = wcsimT->GetEntries();

    if (verbose) printf("Total number of events: %d \n", nevent);
    
    /////////////////////////////////////////////////////////////
    /*TObject *tr;
    WCSimRootTrack *track;
    wcsimT->GetEntry(0);
    wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);

    int ntrack = wcsimrootevent->GetNtrack();
    
    float q, dir[3], pos[3], energy;
    
    for (int k=0; k<ntrack;k++){
        
        tr = (wcsimrootevent->GetTracks())->At(k);
        track = dynamic_cast<WCSimRootTrack*>(tr);
        
        for (int j=0; j<3; j++){
            dir[j] = track->GetDir(j);
            pos[j] = track->GetStart(j);
        }
        energy = track->GetE();
        
        particle_type = track->GetIpnu();
        printf("x, y, z position: (%d, %d, %d) \n", pos[0], pos[1], pos[2]);
        printf("x, y, z direction: (%d, %d, %d) \n", dir[0], dir[1], dir[2]);
        printf("Energy: %f", energy);
        
    }*/
    /////////////////////////////////////////////////////////////
    
    // Info in output images
    // {"true_vertex": [-358.315, 871.359, 1591.82],
    // "true_direction": [0.024953, 0.080551, 0.996438],
    // "vertex": [-376.02, 892.217, 1596.6],
    // "direction": [0.0597496, 0.0283124, 0.997812],
    // "particle_id": 11,
    // "worked_fiTQun": true,
    // "nllcut_fiTQun": 464.777,
    // "nll_e": 5615.52,
    // "energy": 404.834,
    // "data_set": 4,
    // "dist_to_wall": 213.867,
    // "radius": 213.867,
    // "image_width": 1249.73,
    // "phi_vec": [0.428209, -0.90368, 0],
    // "theta_vec": [-0.901702, -0.427272, 0.0661181]}
    
    // Now loop over events
    for (int ev=0; ev<nevent; ev++){
     
        // Read the event from the tree into the WCSimRootEvent instance
        wcsimT->GetEntry(ev);
        
        //ofstream vector_file;
        
        int particle_out_id = (electron)? 11 : 13;
        int number_triggers = wcsimrootsuperevent->GetNumberOfEvents();
        
        if (verbose){
            printf("Event: %d \n", ev);
            printf("Number of Sub-events (triggers) in current event: %d\n \n", number_triggers);
        }
        
        if (number_triggers < 1) continue; //Looking only at first trigger if it exists

        int index = 0;

        //Loop over triggers/subevents
        //Comment this line and //Loop over triggers at end of loop
        //for (int index = 0 ; index < number_triggers; index++){
	
        //Number of negative parent IDs and hits over all hit IDs in CherenkovDigiHit
        int parent_id_neg = 0;
        int total_hits = 0;

        wcsimrootevent = wcsimrootsuperevent->GetTrigger(index);
        
        int ncherenkovdigihits = wcsimrootevent->GetNcherenkovdigihits();
        int ncherenkovhits     = wcsimrootevent->GetNcherenkovhits();

        // Cherenkov hits associated with current trigger of current event
        if(verbose){
            printf("Sub event number: %d \n", index);
            
            printf("Ncherenkovhits %d\n",     ncherenkovhits);
            printf("Ncherenkovdigihits %d\n", ncherenkovdigihits);
            printf("Number of Tracks: %d \n", wcsimrootevent->GetNtrack());
        
        }
            
        if (ncherenkovhits < 1 || ncherenkovdigihits < 1) continue;
        
        //if (not passed_cut(wcsimrootevent->GetNumTubesHit(), particle_vertex)) continue;
        
        //for (i=0;i<(ncherenkovdigihits>4 ? 4 : ncherenkovdigihits);i++){
        for (int i=0;i<ncherenkovdigihits;i++)
        {
            // Loop through elements in the TClonesArray of WCSimRootCherenkovDigHits
	
            TObject *element = (wcsimrootevent->GetCherenkovDigiHits())->At(i);
            TObject *cht;
            TObject *tr;
            WCSimRootTrack *track;
    	
            WCSimRootCherenkovDigiHit *wcsimrootcherenkovdigihit = dynamic_cast<WCSimRootCherenkovDigiHit*>(element);
            WCSimRootPMT pmt;
            WCSimRootCherenkovHitTime *cHitTime;
            
            // Input Vector text file:
            // x_pos, y_pos, z_pos, particle_id, energy, x_dir, y_dir, z_dir
            float q, dir[3], pos[3], energy;
            int tubeid, pmt_x, pmt_y, pmt_z;
            int particle_type, parent_id, t;
            vector<int> hit_id;

            q = wcsimrootcherenkovdigihit->GetQ();
            t = wcsimrootcherenkovdigihit->GetT();
            tubeid = wcsimrootcherenkovdigihit->GetTubeId();
            pmt = geo->GetPMT(tubeid);
	
            hit_id = (wcsimrootcherenkovdigihit->GetPhotonIds());
            //if (i<10 && verbose) printf("Number of Tracks: %d \n", wcsimrootevent->GetNtrack());

            if (hit_id.size()>0){
                if (i==0 && verbose) printf("Number of Hits: %d \n", hit_id.size());
                
                total_hits += hit_id.size();

                for (int id=0; id<hit_id.size();id++){
                    
                    //Within 1 event, have same hit IDs for different digitized hits (i.e. 1 through hit_id.size()

                    cht = (wcsimrootevent->GetCherenkovHitTimes())->At(hit_id[id]);
                    cHitTime = dynamic_cast<WCSimRootCherenkovHitTime*>(cht);
                
                    parent_id = cHitTime->GetParentID();
	        	
                    if (parent_id < 0){
                        //printf("Parent ID: %d", parent_id);
                        //printf(">>CONTINUE \n");
                        parent_id_neg++;
                        continue;
                    }
		    
                    else if (parent_id != 1){
                        printf("ParentID not equal to 1. Value is: %d \n", parent_id);
                    }
		
                    //This was not working
                    tr = (wcsimrootevent->GetTracks())->At(parent_id);
                    
                    //tr =
                    track = dynamic_cast<WCSimRootTrack*>(tr);
                    
                    for (int j=0; j<3; j++){
                        dir[j] = track->GetDir(j);
                        pos[j] = track->GetStart(j);
                    }
                    energy = track->GetE();
                    
                    particle_type = track->GetIpnu();
                    if (i==0 && verbose){
                        printf("x, y, z position: (%d, %d, %d) \n", pos[0], pos[1], pos[2]);
                        printf("x, y, z direction: (%d, %d, %d) \n", dir[0], dir[1], dir[2]);
                        printf("Energy: %f\n", energy);
                        
                        //printf("Hit ID: %d \n", hit_id[id]);
                        
                        //Particle Type is 0 if primary
                        //printf("Particle: %d \n", particle_type);
                        
                        //ID of parent of particle (0 if primary WTF??????????)
                        //printf("Parent ID: %d \n", parent_id);

                    }
                }
            }
            else if (verbose) printf("Hit ID array size <= 0 \n");

            pmt_x = pmt.GetPosition(0);
            pmt_y = pmt.GetPosition(1);
            pmt_z = pmt.GetPosition(2);
 
            if (i == 0 && verbose){
                printf("q, t, tubeid, x, y, z: %f %f %d, %d, %d, %d \n", q, t, tubeid, pmt_x, pmt_y, pmt_z);
                //printf("Track parent ID: %d\n",track->GetParenttype());
            }
                    
        } printf("\n");// End of loop over Cherenkov digihits 

        printf("Number of Parent IDs < 0: %d in %d total hits.\n \n", parent_id_neg, total_hits);
        
        //} printf("\n"); // Loop over triggers
        
        // reinitialize super event between loops.
        
        wcsimrootsuperevent->ReInitialize();
        
    } printf("\n"); // End of loop over events
    
}

bool passed_cut(int num_tubes, double *vertex){
    // If at least 2m from wall and over 160 PMTs hit
    if (num_tubes > 160 && in_cylinder(vertex, 200.0)) return true;
    else return false;
}

bool in_cylinder(float vertex[3]){
    
}

bool in_cylinder(double *vertex, double dist){
    // Check if the vertex is inside the detector cylinder, with an optional cut length off the sides
    // out_log("Checking if vertex is in cylinder");
    
    /*if (vertex.Z() > cut - CAP_HEIGHT && vertex.Z() < CAP_HEIGHT - cut){
        if (sqrt(pow(vertex.X(), 2) + pow(vertex.Y(), 2)) < CYLINDER_RADIUS - cut){
            return true;
        }
    }*/
    
    if (vertex[2] > (dist - CAP_HEIGHT) && vertex[2] < (CAP_HEIGHT - dist)){
        if (sqrt(pow(vertex[0], 2) + pow(vertex[1], 2)) < CYLINDER_RADIUS - dist){
            return true;
        }
    }
    return false;
}


