#include "RecommenderSystem.h"

//**1** explicit constructor and compare func
bool comparator( const std::shared_ptr<Movie>& first,
                 const std::shared_ptr<Movie>& second)
{
    return (*first)<(*second);
}

RecommenderSystem::RecommenderSystem():
_movies_features(std::map<sp_movie,
                 std::vector<double>, equal_func> (comparator)){}

//**2** recommend_by_content - aux funcs
/**
 * copied the rank_map and normalize the ranks
 * @param user
 * @return the normalized ranks in copied rank_map
 */
rank_map normalize (const RSUser& user)
{
    rank_map res = user.get_ranks();
    double avg = 0;
    for(const auto& pair: res)
    {
        avg += pair.second; // summarize the ranks
    }
    avg = avg/(int)res.size(); // // calculates the ranks avg
    for(auto& pair : res)
    { // normalize the vector
        res[pair.first] -= avg;
    }
    return res;
}
/**
 * gets scalar and vector and return its multiply
 * @param scalar
 * @param v
 * @return the multiplied vector
 */
std::vector<double> multiply (double scalar, const std::vector<double>& v)
{
    std::vector<double> res;
    for (const auto& coord : v)
    {
        res.push_back(coord*scalar); // push the feature calculated rate in
    }    // the end in purpose to keep the right feature's order in the vector
    return res;
}
/**
 * gets two vectors and returns theirs sum
 * @param left
 * @param right
 * @return the vectors sum
 */
std::vector<double> vec_sum (const std::vector<double>& left,
                             const std::vector<double>& right)
{
    std::vector<double> res;
    for (int i=0; i<(int)left.size();i++ )
    {
        res.push_back(left[i]+right[i]); // summarize each coord
    }
    return res;
}
/**
 * gets normalize rank_vector of specific user and movie_features map
 * and builds a preferation vector
 * @param normalize_movie_ranks
 * @param movies_features
 * @return the preferation vector
 */
std::vector<double> get_preference_vector
        ( const rank_map& normalize_movie_ranks ,
          std::map<sp_movie,std::vector<double>,equal_func>
                  movies_features)
{
    std::vector<double> final;
    for (int i=0;i<(int)movies_features.size();i++)
    { // initialize the final (in the size of features vector) with 0's
        final.push_back(0.0);
    }
    for (const auto& pair : normalize_movie_ranks)
    { // calculates: sigma(normalize_rank * features_vector)
        final = vec_sum(final,
                         multiply(pair.second,
                                  movies_features[pair.first]) );
    }
    return final;
}
/**
 * gets two vectors and calculates their dot multiply
 * @param v
 * @param w
 * @return the multiply
 */
double vec_dot(const std::vector<double>& v, const std::vector<double>& w)
{
    double dot_sum = 0.0;
    for (int i=0; i<(int)v.size();i++)
    {
        dot_sum += (v[i]*w[i]);
    }
    return dot_sum;
}
/**
 * gets two vectors and calculates the angle between them
 * @param prefer_vector
 * @param features
 * @return the angle
 */
double angle(const std::vector<double>& v,
             const std::vector<double>& w)
{
    double v_norm = sqrt(vec_dot(v,v));
    double w_norm = sqrt(vec_dot(w,w));
    if (v_norm==0 || w_norm==0)
    {
        return 0; // if one of the vectors is the 0 vector the cos(theta)
        // is zero, (they are orthogonal)
    }
    return vec_dot(v, w)/(v_norm*w_norm); // calculates the angle
}
/**
 * gets user and finds the unwatched movie which most similar to the user
 * preferations
 * @param user
 * @return most similar movie
 */
sp_movie RecommenderSystem::recommend_by_content(const RSUser& user) const
{
    rank_map normalize_movie_ranks = normalize(user); // stage level
    std::vector<double> pref_vector = get_preference_vector(
            normalize_movie_ranks, _movies_features);

    bool first = true; // signs if its the first unwatched movie
    sp_movie res;
    double curr_angle;
    for (const auto& pair : _movies_features)
    {
        if (normalize_movie_ranks.find(pair.first) ==
                        normalize_movie_ranks.end())
        {
            if (first)
            { // insert the first unwatched movie as the recommended one
                res = pair.first;
                curr_angle = angle(pref_vector, pair.second);
                first = false;
            }
            else if (angle(pref_vector, pair.second)
                            > curr_angle)
            { // checks if the new movie_features is more similar to pref_vec
                res = pair.first;
                curr_angle = angle(pref_vector, pair.second);
            }
        }
    }
    return res;
}

//**3** predict_movie_score, recommend_by_cf and aux
/**
 * finds the k watched movies which most similar to the given movie
 * @param k
 * @param user_ranks
 * @param movie
 * @param movie_features
 * @return map of the k most similar movies
 */
std::set<sp_movie> find_k_similar(int k, const rank_map& user_ranks,
                          const sp_movie &movie,
                          std::map<sp_movie,std::vector<double>,equal_func>
                          movie_features)
{
    // builds a multimap of <movie, angle> that sort the bigger angle
    // that symbolize similarity in the first
    std::multimap<double,sp_movie,comparator_k> ordered_by_angle ;
    for (const auto& pair : user_ranks)
    {
        ordered_by_angle.insert(
                {angle(movie_features[pair.first],movie_features[movie]) ,
                 pair.first});
    }
    std::set<sp_movie> res;
    int flag = k;
    // inserts the first k movies in the orderd map to res vector
    for (const auto& pair: ordered_by_angle)
    {
        if (!flag)
        {
            break;
        }
        res.insert(pair.second);
        flag--;
    }
    return res;
}

double RecommenderSystem::predict_movie_score (const RSUser &user
                              , const sp_movie &movie,int k)
{
    rank_map user_ranks = user.get_ranks();
    std::set<sp_movie> k_similar = find_k_similar(k,user_ranks,
                                      movie, _movies_features);
    double up = 0.0;
    double down = 0.0;
    for (const auto& curr_movie : k_similar)
    {
        // calculates: sigma(score*angle)
        up += user_ranks[curr_movie] *
            angle(_movies_features[curr_movie], _movies_features[movie]);
        // calculates: sigma(angle)
        down += angle(_movies_features[curr_movie], _movies_features[movie]);
    }
    return up/down;
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
    rank_map user_map = user.get_ranks();
    bool flag = true;
    double max_pred_score;
    sp_movie res;
    for (auto& pair : _movies_features)
    {
        if (user_map.find(pair.first) == user_map.end())
        { // if the movie unwatched
            if (flag)
            { // if its the first which unwatched signing it as the most fit
                max_pred_score = predict_movie_score(user, pair.first,k);
                res = pair.first;
                flag= false;
            }
            else if (predict_movie_score(user,pair.first, k)>max_pred_score)
            { // if not first check if its pred score is better the the curr
                max_pred_score = predict_movie_score(user, pair.first, k);
                res = pair.first;
            }
        }
    }
    return res;
}

//**4** add_movie
sp_movie RecommenderSystem::add_movie(const std::string& name,int year,
                   const std::vector<double>& features)
{
    sp_movie m = std::make_shared<Movie>(name, year);
    _movies_features.insert({m, features});
    return m;
}

//**5** get_movie
sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
    sp_movie temp = std::make_shared<Movie>(name,year);
    auto it = _movies_features.find(temp);
    if (it==_movies_features.end())
    {
        return (sp_movie)nullptr;
    }
    return it->first;
}

//**6**
std::ostream& operator<< (std::ostream& os,
                                 const RecommenderSystem& r_s)
{
    for (const auto& pair : r_s._movies_features)
    {
        os<<*(pair.first);
    }
    return os;
}




