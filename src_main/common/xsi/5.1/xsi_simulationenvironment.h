//*****************************************************************************
/*!
   \file xsi_simulationenvironment.h
   \brief SimulationEnvironment class declaration.

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISIMULATIONENVIRONMENT_H__
#define __XSISIMULATIONENVIRONMENT_H__

#include <xsi_projectitem.h>

namespace XSI {

class Property;
class ActionSource;
//*****************************************************************************
/*! \class SimulationEnvironment xsi_simulationenvironment.h
	\brief The SimulationEnvironment object represents a construct which is used to
	define a simulation.  For now SimulationEnvironment objects are used only
	for RigidBody simulations. A simulation environment is a set of connection
	containers, one for each kind of element in the simulation: the rigid body
	objects, the rigid body constraints and the forces.

	The environment keeps track of the relationships between the objects in the simulation and
	determines onto which objects the dynamics operator is applied. The dynamics
	operator solves the simulation for all elements that are in this environment.
	As well, a time control determines how the simulation in this environment is
	played back.

	An environment is created as soon as you create a RigidBody object, which you can
	achieve by using the CreateActiveRigidBody or CreatePassiveRigidBody commands.
	You can also explicitly create an environment by using the CreateEnvironment
	command.  SimulationEnvironment objects can be retrieved using
	Scene::GetActiveSimulationEnvironment or Scene::GetSimulationEnvironments.

	\sa Scene::GetSimulationEnvironments, Scene::GetActiveSimulationEnvironments,
		\xl SetCurrentEnvironment

	\since 4.2

	\eg Illustrates how to create a cached RigidBody simulation and apply that
	cache to animate the simulated objects directly without using the simulation.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();
		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface",L"myCone",myCone);

		Model myModel;
		root.AddModel(CRefArray(),L"myModel",myModel);

		X3DObject myGrid;
		myModel.AddGeometry( L"Grid", L"MeshSurface",L"myGrid",myGrid);

		// Move the cone
		myCone.PutParameterValue(L"posy",6.0);

		// Modify the grid
		myGrid.PutParameterValue(L"ulength",24l);
		myGrid.PutParameterValue(L"vlength",24l);
		myGrid.PutParameterValue(L"subdivu",24l);
		myGrid.PutParameterValue(L"subdivv",24l);

		// The following line creates the environment.
		CValueArray args(1);
		CValue outArg;
		args[0] = myGrid.GetRef();
		app.ExecuteCommand(L"CreatePassiveRigidBody",args,outArg);

		args[0] = myCone.GetRef();
		app.ExecuteCommand(L"CreateActiveRigidBody",args,outArg);

		args[0] = L"Gravity";
		app.ExecuteCommand(L"CreateForce",args,outArg);

		SimulationEnvironment currEnv = app.GetActiveProject().GetActiveScene().GetActiveSimulationEnvironment();
		Property simTimeControl = currEnv.GetSimulationTimeControl();

		// Turn the caching on.
		simTimeControl.PutParameterValue(L"caching",true);
		simTimeControl.PutParameterValue(L"playmode",0l);//Standard play mode.

		// Simulate
		// To make sure that all frames gets simulated
		args.Resize(0);
		for(LONG i =0 ; i < 100 ; i++)
		{
			app.ExecuteCommand(L"NextFrame",args,outArg);
			app.ExecuteCommand(L"Refresh",args,outArg);
			app.LogMessage(L"Frame : " + CValue(i).GetAsText());
		}

		// Get models of the scene
		CRefArray refModels;
		refModels.Add(root);
		refModels.Add(myModel);

		ApplyCurrentEnvironmentCacheToSimulateObjectForModels(refModels);

		// The FCurve animated objects
		app.ExecuteCommand(L"PlayForwardsFromStart",args,outArg);

		// This function takes the cached simulation and copies it directly
		// on the driven objects as FCurve. It also turns off the simulation
		// so that we could
		void ApplyCurrentEnvironmentCacheToSimulateObjectForModels(CRefArray& in_models)
		{
			Application app;

			SimulationEnvironment currEnv = app.GetActiveProject().GetActiveScene().GetActiveSimulationEnvironment();

			// Deactivate the simulation so that objects are driven by their
			// animation.
			Property simTimeControl= currEnv.GetSimulationTimeControl();
			simTimeControl.PutParameterValue(L"Active",false);

			for (LONG i = 0; i < in_models.GetCount();++i)
			{
				Model currModel = in_models[i];
				ActionSource currCache = currEnv.FindCacheForModel(currModel);
				CRefArray sourceItems = currCache.GetItems();

				for(LONG j = 0; j < sourceItems.GetCount(); ++j)
				{
					AnimationSourceItem currSourceItem = sourceItems[j];
					CString strTarget = currSourceItem.GetTarget();
					if(!currModel.IsEqualTo(app.GetActiveSceneRoot()))
					{
						// ActionSource keeps the relative name of an
						// animated parameter. We need to prepend the
						// model name if the model owner is not the scene
						// root in order to fetch the parameter.
						strTarget = currModel.GetName() + L"." + strTarget;
					}

					CRef tmpRef;
					tmpRef.Set(strTarget);

					Parameter targetParam(tmpRef);

					// The RigidBody simulation caches linear acceleration which
					// is not animatable. We won't be copying those values on
					// the global transform.
					if (targetParam.GetCapabilities() & siAnimatable)
					{
						FCurve cacheFCurve = currSourceItem.GetSource();
						CopyFCurveOnParameter(cacheFCurve, targetParam);
					}
				}
			}
		}

		// This function creates a copy of an FCurve on a parameter by
		// creating a new FCurve on the parameter and adding all keys.
		void CopyFCurveOnParameter(FCurve& in_FCurve,Parameter& in_Parameter)
		{
			FCurve newFCurve;
			in_Parameter.AddFCurve(siStandardFCurve,newFCurve);
			CRefArray fcKeys = in_FCurve.GetKeys();

			in_FCurve.BeginEdit();

			for(LONG i = 0;i < fcKeys.GetCount();++i)
			{
				FCurveKey currKey(fcKeys[i]);
				newFCurve.AddKey(currKey.GetTime(),currKey.GetValue());
			}
			in_FCurve.EndEdit();
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL SimulationEnvironment : public ProjectItem
{
public:
	/*! Default constructor. */
	SimulationEnvironment();

	/*! Default destructor. */
	~SimulationEnvironment();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	SimulationEnvironment(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	SimulationEnvironment(const SimulationEnvironment& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new SimulationEnvironment object.
	*/
	SimulationEnvironment& operator=(const SimulationEnvironment& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new SimulationEnvironment object.
	*/
	SimulationEnvironment& operator=(const CRef& in_ref);

	/*! Returns all containers
	\since 4.2
	*/
	CRefArray GetContainers()const;

	/*! Returns an array of ActionSource objects which are used for the caching of the
	simulation (see \b Environments > \b Environment > \b Caches under the Scene).
	There is one cache for each model containing a simulation environment.

	As soon as you add an object to the simulation environment a cache for the model
	of the object will be created. The cache will only be populated if the simulation
	time control caching parameter is set to true and there is a frame change.
	\sa SimulationEnvironment::FindCacheForModel
	\since 4.2
	*/
	CRefArray GetCaches()const;

	/*! Returns all simulation operators used by this environment.
	\since 4.2
	*/
	CRefArray GetOperators()const;

	/*! Returns the simulation time control of this environment.
	\since 4.2
	*/
	Property	GetSimulationTimeControl()const;

	/*! Returns the ActionSource which is used for caching the simulation of a given model. If there
	is no cache the function returns an invalid ActionSource object. You can verify the validity
	of an object using CBase::IsValid.

	Caches are stored in the \c Environments > \c Environment > \c Caches folder under the Scene.

	\param in_model the Model for which we want to retrieve the cache.
	\since 4.2
	*/
	ActionSource FindCacheForModel(Model& in_model)const;

private:
	SimulationEnvironment * operator&() const;
	SimulationEnvironment * operator&();
};

};

#endif // __XSISIMULATIONENVIRONMENT_H__
