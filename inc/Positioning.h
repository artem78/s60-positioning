/*
 ============================================================================
 Name		: Positioning.h
 Author	  : artem78
 Version	 : 1.0
 Copyright   : 
 Description : Receiving location info
 ============================================================================
 */

//TODO: Return satellites info 

#ifndef POSITIONING_H
#define POSITIONING_H

#include <e32base.h>	// For CActive, link against: euser.lib
//#include <e32std.h>		// For RTimer, link against: euser.lib
#include <lbs.h>
//#pragma comment(lib, "c:\\Symbian\\9.2\\S60_3rd_FP1\\Epoc32\\release\\winscw\\udeb\\lbs.lib") // TODO: Change path to local
#include <e32math.h>
#include "Logger.h"


const TInt KSecond = 1000000;
const TInt KDefaultPositionUpdateInterval = /* 5 * */ KSecond;
const TInt KDefaultPositionUpdateTimeOut = /*15 * KSecond*/ KDefaultPositionUpdateInterval * 5;


class MPositionListener
	{
public:
	virtual void OnPositionUpdated() = 0;
	virtual void OnPositionPartialUpdated() = 0;
	virtual void OnConnected() = 0;
	virtual void OnDisconnected() = 0;
	virtual void OnError(TInt aErrCode) = 0;
	};


class CPositionRequestor : public CActive
	{
public:
	// Cancel and destroy
	~CPositionRequestor();

	// Two-phased constructor.
	static CPositionRequestor* NewL(MPositionListener *aListener,
			TTimeIntervalMicroSeconds aUpdateInterval = TTimeIntervalMicroSeconds(KDefaultPositionUpdateInterval),
			TTimeIntervalMicroSeconds aUpdateTimeOut = TTimeIntervalMicroSeconds(KDefaultPositionUpdateTimeOut));

	// Two-phased constructor.
	static CPositionRequestor* NewLC(MPositionListener *aListener,
			TTimeIntervalMicroSeconds aUpdateInterval = TTimeIntervalMicroSeconds(KDefaultPositionUpdateInterval),
			TTimeIntervalMicroSeconds aUpdateTimeOut = TTimeIntervalMicroSeconds(KDefaultPositionUpdateTimeOut));

public:
	// New functions
	// Function for making the initial request
	void StartL();

//private:
protected: // ToDo: Make all private protected?
	// C++ constructor
	CPositionRequestor(MPositionListener *aListener,
			TTimeIntervalMicroSeconds aUpdateInterval,
			TTimeIntervalMicroSeconds aUpdateTimeOut);
	
//private:

	// Second-phase constructor
	void ConstructL();

//private:
protected:
	// From CActive
	// Handle completion
	void RunL(); // ToDo: Is L really needed?
	
private:
	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	//TInt RunError(TInt aError);

public:
	enum TPositionRequestorState
		{
		EStopped,				// Positioning is disabled
		EPositionNotRecieved, /*Pending*/	// Position is not yet recieved
		EPositionRecieved		// Position recieved
		};


public:
	/*inline*/ TInt State() const;
	/*inline*/ TBool IsRunning() const; // ToDo: Why "Undefined symbol" error when inline?
	/*inline*/ TBool IsPositionRecieved() const;
	TPositionInfo* LastKnownPositionInfo();
	TPositionInfo* PrevLastKnownPositionInfo();
	//void LastKnownPositionInfo(TPositionInfo &aPosInfo);
	//void PrevLastKnownPositionInfo(TPositionInfo &aPosInfo);
	
private:
	TInt /*(TODO: TPositionRequestorState ?)*/ iState; // State of the active object
	
	MPositionListener *iListener;
	
	RPositionServer iPosServer;
protected:
	RPositioner iPositioner;
	
//protected:
	TPositionInfo* iLastPosInfo;
	TPositionInfo* iPrevLastPosInfo;
	
private:
	
	void SetState(TInt aState);
	
protected:
	TPositionUpdateOptions iUpdateOptions;

	};



/*const TUint KMaxSpeedCalculationPeriod = 60;
const TUint KDistanceBetweenPoints = 30;
const TUint KPositionMinUpdateInterval = 1;
const TUint KPositionMaxUpdateInterval = 30;*/

// Forward declaration
class CPointsCache;

/*
 * Automaticaly set position update interval depending on current speed
 * to preserve approximately equal distance between points 
 */
class CDynamicPositionRequestor: public CPositionRequestor
	{
public:
	~CDynamicPositionRequestor();
	
	static CDynamicPositionRequestor* NewL(MPositionListener *aListener);

	// Two-phased constructor.
	static CDynamicPositionRequestor* NewLC(MPositionListener *aListener);
	
	/*inline*/ TTimeIntervalMicroSeconds UpdateInterval();
	
private:
	// C++ constructor
	CDynamicPositionRequestor(MPositionListener *aListener);
	
	// Second-phase constructor
	void ConstructL();
	
	// From CActive
	// Handle completion
	void RunL(); // ToDo: Is L really needed?

	//TReal32 GetMaxSpeedByPeriod(); // Complex getter
	//TReal32 MaxSpeedDuringPeriod();
	
	CPointsCache* iPointsCache;
	
	};

/*
 * Stores positions for the last period and return max speed
 */
class CPointsCache: public CBase // ToDo: Optimize
	{
public:
	CPointsCache(TTimeIntervalMicroSeconds aPeriod);
	~CPointsCache();
	
	void AddPoint(const TPosition &aPos);
	//TReal32 GetMaxSpeed();
	
	/* Get max speed from saved points.
	 * Return KErrNone if there`s no error and error code
	 * when there are not enouth points to calculate speed (less then 2). 
	 * @return a Symbian OS error code.
	 */
	TInt GetMaxSpeed(TReal32 &aSpeed);
private:
	TTimeIntervalMicroSeconds iPeriod;
	RArray<TPosition> iPoints; // ToDo: What about CCirBuf?
		// ToDo: Set granylarity
	
	void ClearOldPoints();
	};


#endif // POSITIONING_H
