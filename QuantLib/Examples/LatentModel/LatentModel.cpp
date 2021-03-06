/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2014 Jose Aparicio

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/quantlib.hpp>

#include <boost/timer.hpp>
#include <boost/make_shared.hpp>
#include <boost/function.hpp>

#include <iostream>
#include <iomanip>

using namespace std;
using namespace QuantLib;

#ifdef BOOST_MSVC
#  ifdef QL_ENABLE_THREAD_SAFE_OBSERVER_PATTERN
#    include <ql/auto_link.hpp>
#    define BOOST_LIB_NAME boost_system
#    include <boost/config/auto_link.hpp>
#    undef BOOST_LIB_NAME
#    define BOOST_LIB_NAME boost_thread
#    include <boost/config/auto_link.hpp>
#    undef BOOST_LIB_NAME
#  endif
#endif


#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    Integer sessionId() { return 0; }

}
#endif


/* This sample code shows basic usage of a Latent variable model.
   The data and correlation problem presented is the same as in:
     'Modelling Dependent Defaults: Asset Correlations Are Not Enough!'
     Frey R., A. J. McNeil and M. A. Nyfeler RiskLab publications March 2001
*/
int main(int, char* []) {

    try {

        boost::timer timer;
        std::cout << std::endl;

        Calendar calendar = TARGET();
        Date todaysDate(19, March, 2014);
        // must be a business day
        todaysDate = calendar.adjust(todaysDate);

        Settings::instance().evaluationDate() = todaysDate;


        /* --------------------------------------------------------------
                        SET UP BASKET PORTFOLIO
        -------------------------------------------------------------- */
        // build curves and issuers into a basket of three names
        std::vector<Real> hazardRates(3, -std::log(1.-0.01));
        std::vector<std::string> names;
        for(Size i=0; i<hazardRates.size(); i++)
            names.push_back(std::string("Acme") + 
                boost::lexical_cast<std::string>(i));
        std::vector<Handle<DefaultProbabilityTermStructure> > defTS;
        for(Size i=0; i<hazardRates.size(); i++)
            defTS.push_back(Handle<DefaultProbabilityTermStructure>(
                boost::make_shared<FlatHazardRate>(0, TARGET(), hazardRates[i], 
                    Actual365Fixed())));
        std::vector<Issuer> issuers;
        for(Size i=0; i<hazardRates.size(); i++) {
            std::vector<QuantLib::Issuer::key_curve_pair> curves(1, 
                std::make_pair(NorthAmericaCorpDefaultKey(
                    EURCurrency(), QuantLib::SeniorSec,
                    Period(), 1. // amount threshold
                    ), defTS[i]));
            issuers.push_back(Issuer(curves));
        }

        boost::shared_ptr<Pool> thePool = boost::make_shared<Pool>();
        for(Size i=0; i<hazardRates.size(); i++)
            thePool->add(names[i], issuers[i], NorthAmericaCorpDefaultKey(
                    EURCurrency(), QuantLib::SeniorSec, Period(), 1.));

        std::vector<DefaultProbKey> defaultKeys(hazardRates.size(), 
            NorthAmericaCorpDefaultKey(EURCurrency(), SeniorSec, Period(), 1.));
        // Recoveries are irrelevant in this example but must be given as the 
        //   lib stands.
        std::vector<boost::shared_ptr<RecoveryRateModel> > rrModels(
            hazardRates.size(), boost::make_shared<ConstantRecoveryModel>(
            ConstantRecoveryModel(0.5, SeniorSec)));
        boost::shared_ptr<Basket> theBskt = boost::make_shared<Basket>(
            todaysDate, names, std::vector<Real>(hazardRates.size(), 100.), 
            thePool);
        /* --------------------------------------------------------------
                        SET UP JOINT DEFAULT EVENT LATENT MODELS
        -------------------------------------------------------------- */
        // Latent model factors, corresponds to the first entry in Table1 of the
        //   publication mentioned. It is a single factor model
        std::vector<std::vector<Real> > fctrsWeights(hazardRates.size(), 
            std::vector<Real>(1, std::sqrt(0.1)));
        // --- Default Latent models -------------------------------------
        // Gaussian integrable joint default model:
        boost::shared_ptr<GaussianDefProbLM> lmG(new 
            GaussianDefProbLM(fctrsWeights, 
            LatentModelIntegrationType::GaussianQuadrature,
			GaussianCopulaPolicy::initTraits() // otherwise gcc screams
			));
        // Define StudentT copula
        // this is as far as we can be from the Gaussian, 2 T_3 factors:
        std::vector<Integer> ordersT(2, 3);
        TCopulaPolicy::initTraits iniT;
        iniT.tOrders = ordersT;
        // StudentT integrable joint default model:
        boost::shared_ptr<TDefProbLM> lmT(new TDefProbLM(fctrsWeights, 
            // LatentModelIntegrationType::GaussianQuadrature,
            LatentModelIntegrationType::Trapezoid,
            iniT));

        // --- Default Loss models ----------------------------------------
        // Gaussian random joint default model:
        Size numSimulations = 100000;
        // Size numCoresUsed = 4;
        // Sobol, many cores
        boost::shared_ptr<DefaultLossModel> rdlmG(
            boost::make_shared<RandomDefaultLM<GaussianCopulaPolicy> >(lmG, 
                std::vector<Real>(), numSimulations, 1.e-6, 2863311530));
        // StudentT random joint default model:
        boost::shared_ptr<DefaultLossModel> rdlmT(
            boost::make_shared<RandomDefaultLM<TCopulaPolicy> >(lmT, 
            std::vector<Real>(), numSimulations, 1.e-6, 2863311530));

        /* --------------------------------------------------------------
                        DUMP SOME RESULTS
        -------------------------------------------------------------- */
        /* Default correlations in a T copula should be below those of the 
        gaussian for the same factors.
        The calculations on the MC show dispersion on both copulas (thats
        ok) and too large values with very large dispersions on the T case.
        Computations are ok, within the dispersion, for the gaussian; compare
        with the direct integration in both cases.
        However the T does converge to the gaussian value for large value of
        the parameters.
        */
        Date calcDate(TARGET().advance(Settings::instance().evaluationDate(), 
            Period(120, Months)));
        std::vector<Probability> probEventsTLatent, probEventsGLatent, 
            probEventsTRandLoss, probEventsGRandLoss;
        //
        lmT->resetBasket(theBskt);
        for(Size numEvts=0; numEvts <=theBskt->size(); numEvts++) {
            probEventsTLatent.push_back(lmT->probAtLeastNEvents(numEvts, 
                calcDate));
         }
        //
        lmG->resetBasket(theBskt);
        for(Size numEvts=0; numEvts <=theBskt->size(); numEvts++) {
            probEventsGLatent.push_back(lmG->probAtLeastNEvents(numEvts, 
                calcDate));
         }
        //
        theBskt->setLossModel(rdlmT);
        for(Size numEvts=0; numEvts <=theBskt->size(); numEvts++) {
            probEventsTRandLoss.push_back(theBskt->probAtLeastNEvents(numEvts, 
                calcDate));
         }
        //
        theBskt->setLossModel(rdlmG);
        for(Size numEvts=0; numEvts <=theBskt->size(); numEvts++) {
            probEventsGRandLoss.push_back(theBskt->probAtLeastNEvents(numEvts, 
                calcDate));
         }

        Date correlDate = TARGET().advance(
            Settings::instance().evaluationDate(), Period(12, Months));
        std::vector<std::vector<Real> > correlsGlm, correlsTlm, correlsGrand, 
            correlsTrand;
        //
        lmG->resetBasket(theBskt);
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            std::vector<Real> tmp;
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                tmp.push_back(lmG->defaultCorrelation(correlDate, 
                    iName1, iName2));
            correlsGlm.push_back(tmp);
        }
        //
        lmT->resetBasket(theBskt);
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            std::vector<Real> tmp;
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                tmp.push_back(lmT->defaultCorrelation(correlDate, 
                    iName1, iName2));
            correlsTlm.push_back(tmp);
        }
        //
        theBskt->setLossModel(rdlmG);
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            std::vector<Real> tmp;
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                tmp.push_back(theBskt->defaultCorrelation(correlDate, 
                    iName1, iName2));
            correlsGrand.push_back(tmp);
        }
        //
        theBskt->setLossModel(rdlmT);
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            std::vector<Real> tmp;
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                tmp.push_back(theBskt->defaultCorrelation(correlDate, 
                    iName1, iName2));
            correlsTrand.push_back(tmp);
        }



        std::cout << 
            " Gaussian versus T prob of extreme event (random and integrable)-" 
            << std::endl;
        for(Size numEvts=0; numEvts <=theBskt->size(); numEvts++) {
            std::cout << "-Prob of " << numEvts << " events... " <<
                probEventsGLatent[numEvts] << " ** " << 
                probEventsTLatent[numEvts] << " ** " << 
                probEventsGRandLoss[numEvts]<< " ** " << 
                probEventsTRandLoss[numEvts] 
            << std::endl;
        }

        cout << endl;
        cout << "-- Default correlations G,T,GRand,TRand--" << endl;
        cout << "-----------------------------------------" << endl;
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                cout << 
                    correlsGlm[iName1][iName2] << " , ";
            ;
                cout << endl;
        }
        cout << endl;
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                cout << 
                    correlsTlm[iName1][iName2] << " , ";
            ;
                cout << endl;
        }
        cout << endl;
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                cout << 
                    correlsGrand[iName1][iName2] << " , ";
            ;
                cout << endl;
        }
        cout << endl;
        for(Size iName1=0; iName1 <theBskt->size(); iName1++) {
            for(Size iName2=0; iName2 <theBskt->size(); iName2++)
                cout << 
                    correlsTrand[iName1][iName2] << " , ";
            ;
                cout << endl;
        }



        Real seconds  = timer.elapsed();
        Integer hours = Integer(seconds/3600);
        seconds -= hours * 3600;
        Integer minutes = Integer(seconds/60);
        seconds -= minutes * 60;
        cout << "Run completed in ";
        if (hours > 0)
            cout << hours << " h ";
        if (hours > 0 || minutes > 0)
            cout << minutes << " m ";
        cout << fixed << setprecision(0)
             << seconds << " s" << endl;

        return 0;
    } catch (exception& e) {
        cerr << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "unknown error" << endl;
        return 1;
    }
}

