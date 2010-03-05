/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

////////////////////////////////////////////////////////////////////////////////
///
/// \file		svm_classifier.h
/// \brief		Prototype of class "SVMClassifier"
///
/// \date		2009-07-29
/// \author		Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SVM_CLASSIFIER_H
#define SVM_CLASSIFIER_H

//--- Program header ---------------------------------------------------------//
#include "feature_extractor.h"
#include "log.h"
#include "otb_svmop_common.h"

//--- Standard header --------------------------------------------------------//
#include <fstream>

//--- Misc. header -----------------------------------------------------------//

const uint SVM_CLASSIFIER_DEFAULT_NUMBER_OF_CLASSES = 2;
const bool SVM_CLASSIFIER_CALCULATE_EXTREMA = true;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief Classifier class using a support vector machine
///
////////////////////////////////////////////////////////////////////////////////
class SVMClassifier
{
	
	public:

		//--- Constructor/Destructor -----------------------------------------//
		SVMClassifier();
		~SVMClassifier();

		//--- Const Methods --------------------------------------------------//
        LabelImageType::Pointer getClassificationResult() const;
// 		double getFeatureMin() const;
// 		double getFeatureMax() const;
		bool saveClassificationResult(const std::string&) const;
		bool saveData(const std::string&) const;
        bool saveScaling(const std::string&) const;
		bool saveModel(const std::string&) const;

		//--- Methods --------------------------------------------------------//
		bool loadModel(const std::string&);
        bool loadScaling(const std::string&);

		bool scaleFeatures(const bool& = false,
						   const double& = -1.0, const double& = 1.0);
		
		void setFeatures(const FeaturePointSetType::Pointer);
		void setLabels(const LabelPointSetType::Pointer);
		void setLabelImageSize(const LabelImageType::SizeType&);
		void setNumberOfClasses(const uint&);

		bool classify();
		bool train();

		bool loadConfig(){return true;}
		
	private:
	
		//--- Methods --------------------------------------------------------//
		TestFeaturePointSetType::Pointer convertPointSet(
										 FeaturePointSetType::Pointer);
		
		//--- Variables ------------------------------------------------------//
		ClassifierType::Pointer			m_pClassifier;	///< Instance of LibSVM wrapper class (classification)
		EstimatorType::Pointer			m_pEstimator;	///< Instance of LibSVM wrapper class (training)
		FeaturePointSetType::Pointer	m_pFeatures;	///< Contains the extracted features
		LabelPointSetType::Pointer		m_pLabels;		///< Contains the labels
		ModelType::Pointer				m_pModel;		///< SVM model
		
		LabelImageType::Pointer		    m_pLabelImage;	        ///< Label image resulting from classification
		LabelImageType::SizeType	    m_LabelImageSize;       ///< Contains size of label image
		std::vector<ImageFloatType::Pointer> m_Probabilities;   ///< Class probabilities for each pixel

		uint			m_unNumberOfClasses;			///< Number of classes
		
		double			m_fMin;							///< Minimum feature value
		double			m_fMax;							///< Maximum feature value

		bool			m_bGotClassificationResult;		///< Flags if classification was done
		bool			m_bGotFeatures;					///< Flags if features were passed
		bool			m_bGotLabels;					///< Flags if labels were passed
		bool			m_bGotModel;					///< Flags if a model exists
        bool            m_bGotScaling;                  ///< Flags if features are already scaled
		bool			m_bGotSize;						///< Flags if size of label image is known
		
		DEBUG(
			static uint m_unNoFVFiles;					///< Indicates the number of written feature vector files
		);
};

//--- Implementation is done here for inlining -------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add features for classification
///
/// \param _pFeatures PointSet of features
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setFeatures(const FeaturePointSetType::Pointer _pFeatures)
{
	METHOD_ENTRY("SVMClassifier::setFeatures(const FeaturePointSetType::Pointer)");

	m_pFeatures = _pFeatures;
	m_bGotFeatures = true;

	METHOD_EXIT("SVMClassifier::setFeatures(const FeaturePointSetType::Pointer)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add labels for training
///
/// \param _pLabels PointSet of labels
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setLabels(const LabelPointSetType::Pointer _pLabels)
{
	METHOD_ENTRY("SVMClassifier::setFeatures(const LabelPointSetType::Pointer)");

	m_pLabels = _pLabels;
	m_bGotLabels = true;

	METHOD_EXIT("SVMClassifier::setFeatures(const LabelPointSetType::Pointer)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets size for label image
///
/// \param _pLabels PointSet of labels
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setLabelImageSize(const LabelImageType::SizeType& _Size)
{
	METHOD_ENTRY("SVMClassifier::setLabelImageSize(const LabelImageType::SizeType)");

	m_LabelImageSize = _Size;
	m_bGotSize = true;

	METHOD_EXIT("SVMClassifier::setLabelImageSize(const LabelImageType::SizeType)");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Sets the number of classes for training
///
/// \param _unNOC
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setNumberOfClasses(const uint& _unNOC)
{
	METHOD_ENTRY("SVMClassifier::setNumberOfClasses(const uint&)");

	m_unNumberOfClasses = _unNOC;

	METHOD_EXIT("SVMClassifier::setNumberOfClasses(const uint&)");
}

#endif
