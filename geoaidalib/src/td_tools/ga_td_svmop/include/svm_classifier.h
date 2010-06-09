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
/// \file       svm_classifier.h
/// \brief      Prototype of class "SVMClassifier"
///
/// \date       2009-07-29
/// \author     Torsten Bueschenfeld (bfeld@tnt.uni-hannover.de)
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
        SVMClassifier(FeaturesType&);
        ~SVMClassifier();

        //--- Const Methods --------------------------------------------------//
        LabelImageType::Pointer getLabelImage() const;
        bool saveClassificationResult(const std::string&) const;
        bool saveData(const std::string&) const;
        bool saveScaling(const std::string&) const;
        bool saveModel(const std::string&) const;

        //--- Methods --------------------------------------------------------//
        bool loadModel(const std::string&);
        bool loadScaling(const std::string&);

        bool scaleFeatures(const bool& = false,
                        const double& = -1.0, const double& = 1.0);

        void addLabels(const LabelsType&);
        void addFeatures(const FeaturesType&);
                        
        void setConfigurator(Configurator* const);
        void setFeatures(const FeaturesType&);
        void setLabels(const LabelsType&);
        void setLabelImageSize(const LabelImageType::SizeType&);
        void setNumberOfClasses(const uint&);

        bool applyUncertaintyOnLabelImage();
        bool calculateDistanceMaps();
        bool calculateUncertainty();
        bool joinProbabilities(const std::string&, const std::string&,
                               const std::string&, const std::string&);
        bool classify();
        bool train();
        
        void createLabelImageFromProbabilities();

    private:
    
        //--- Methods --------------------------------------------------------//
        SVMClassifier();
                                        
        //--- Variables ------------------------------------------------------//
        FeaturesType&                   m_Features;         ///< Contains the extracted features
        LabelsType                      m_Labels;           ///< Contains the labels
        ModelType::Pointer              m_pModel;           ///< SVM model
        
        LabelImageType::Pointer         m_pLabelImage;      ///< Label image resulting from classification
        LabelImageType::SizeType        m_LabelImageSize;   ///< Contains size of label image
        std::vector<ImageFloatType::Pointer>
                                        m_Probabilities;    ///< Class probabilities for each pixel
        std::vector<DistanceMapImageType::Pointer>
                                        m_DistanceMaps;     ///< Distances to reliable regions for each pixel
        ImageFloatType::Pointer         m_pUncertainty;     ///< Uncertainty of classification result
        
        std::vector< int >              m_LabelMap;         ///< Values for labels
        
        uint                    m_unNumberOfClasses;        ///< Number of classes
        
        std::vector<float>      m_Min;                      ///< Minimum feature value
        std::vector<float>      m_Max;                      ///< Maximum feature value

        bool            m_bGotClassificationResult;     ///< Flags if classification was done
        bool            m_bGotDistanceMaps;             ///< Flags if distance maps were calculated
        bool            m_bGotFeatures;                 ///< Flags if features were passed
        bool            m_bGotLabels;                   ///< Flags if labels were passed
        bool            m_bGotModel;                    ///< Flags if a model exists
        bool            m_bGotScaling;                  ///< Flags if features are already scaled
        bool            m_bGotSize;                     ///< Flags if size of label image is known
        
        Configurator*   m_pConfiguration;               ///< Configuration of parameters
        
        DEBUG(
            static uint m_unNoFVFiles;                  ///< Indicates the number of written feature vector files
        );
};

//--- Implementation is done here for inlining -------------------------------//

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Get the parameter configuration
///
/// \param _pFeatures PointSet of features
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setConfigurator(Configurator* const _pConfigurator)
{
    METHOD_ENTRY("SVMClassifier::setConfigurator");

    m_pConfiguration = _pConfigurator;

    METHOD_EXIT("SVMClassifier::setConfigurator");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add features for classification
///
/// \param _pFeatures PointSet of features
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setFeatures(const FeaturesType& _Features)
{
    METHOD_ENTRY("SVMClassifier::setFeatures");

    m_Features = _Features;
    m_bGotFeatures = true;

    METHOD_EXIT("SVMClassifier::setFeatures");
}

///////////////////////////////////////////////////////////////////////////////
///
/// \brief Add labels for training
///
/// \param _pLabels PointSet of labels
///
///////////////////////////////////////////////////////////////////////////////
inline void SVMClassifier::setLabels(const LabelsType& _Labels)
{
    METHOD_ENTRY("SVMClassifier::setLabels");

    m_Labels = _Labels;
    m_bGotLabels = true;

    METHOD_EXIT("SVMClassifier::setLabels");
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
    METHOD_ENTRY("SVMClassifier::setLabelImageSize");

    m_LabelImageSize = _Size;
    m_bGotSize = true;

    METHOD_EXIT("SVMClassifier::setLabelImageSize");
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
    METHOD_ENTRY("SVMClassifier::setNumberOfClasses");

    m_unNumberOfClasses = _unNOC;

    METHOD_EXIT("SVMClassifier::setNumberOfClasses");
}

#endif
