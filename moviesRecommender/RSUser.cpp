//
// Created on 2/20/2022.
//

// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"
// implement your cpp code here

RSUser::RSUser(const std::string& name, rank_map ranks,
                sp_rs& rec_sys):
     _rec_sys(rec_sys)
    {
        _name = name;
        _ranks = std::move(ranks);
    }

const std::string& RSUser::get_name() const
{
    return this->_name;
}

void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
    sp_movie to_add = _rec_sys->add_movie(name,year,features);
    _ranks.insert({to_add, rate});
}
rank_map RSUser::get_ranks() const
{
    return _ranks;
}

std::ostream& operator<<(std::ostream& os, const RSUser& rs_user)
{
    os<<"name: "<<rs_user._name<<"\n"<<*(rs_user._rec_sys)<<std::endl;
    return os;
}

sp_movie RSUser::get_recommendation_by_content() const
{
    return _rec_sys->recommend_by_content(*this);
}

double RSUser::get_prediction_score_for_movie(const std::string& name,
                                              int year,int k) const
{
    return _rec_sys->predict_movie_score
            (*this,std::make_shared<Movie>(name, year), k);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
    return _rec_sys->recommend_by_cf(*this,k);
}

