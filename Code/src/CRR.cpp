#include "CRR.hpp"
#include "JsonParser.hpp"

CRR::CRR(char * source, char * output) {
    loadModelJson(source, false);
    greeks.estimateGreeks(*this);
    saveModelJson(output);
}

double CRR::crrOptionPrice() {
    updateModel();
    for(int i = 0; i < N + 1; ++i) {
        optionValues.push_back(0);
    }

    double p = (std::exp(r * dt) - d) / (u - d);
    double discount = std::exp(-r * dt);

    if (p <= 0.0 || p >= 1.0) {
        std::cerr << "ERROR : Arbitrage\n";
        return -1.;
    }
    for (int i = 0; i < N + 1; ++i) {
        double ST = S0 * std::pow(u, N - i) * std::pow(d, i);
        if (oType == OptionType::CALL) {
            optionValues[i] = std::max(0., ST - K);
        } else {
            optionValues[i] = std::max(0., K - ST);
        }
    }
    double continuation;
    for (int j = N - 1; j >= 0; --j) {
        for (int i = 0; i <= j; ++i) {
            continuation = discount * (p * optionValues[i] + (1.0 - p) * optionValues[i + 1]);
            if (eType == ExerciseType::EUROPEAN) {
                optionValues[i] = continuation;
                
            } else {
                double S_current = S0 * std::pow(u, j - i) * std::pow(d, i);
                double value = (oType == OptionType::CALL) ? std::max(0.0, S_current - K) : std::max(0.0, K - S_current);
                optionValues[i] = std::max(continuation, value);
            }
        }
    }

    riskNeutral = p;
    result = optionValues[0];
    computed = true;
    return optionValues[0];
}

void CRR::updateModel() {
    dt = T / N;
    u = std::exp(sigma * std::sqrt(dt));
    d = 1.0 / u;
}

void CRR::setSigma(double vol) {
    sigma = vol;
    updateModel();
}

double CRR::getS0() const { return S0; }
double CRR::getK() const { return K; }
double CRR::getR() const { return r; }
double CRR::getSigma() const { return sigma; }
double CRR::getU() const { return u; }
double CRR::getD() const { return d; }
double CRR::getDt() const { return dt; }
double CRR::getT() const { return T; }
int CRR::getN() const { return N; }
bool CRR::getComputed() const { return computed; }
double CRR::getResult() const { return result; }
ExerciseType CRR::getEType() const { return eType; };
OptionType CRR::getOType() const { return oType; };
double CRR::getRiskNeutral() const { return riskNeutral; };


void CRR::setS0(double s0) { S0 = s0; }
void CRR::setK(double k) { K = k; }
void CRR::setR(double rate) { r = rate; }
void CRR::setT(double t) { T = t; }

void CRR::setN(int n) { N = n; }
void CRR::setComputed(bool b) { computed = b; }
void CRR::setResult(double r) { result = r; }
void CRR::setEType(ExerciseType e) { eType = e; };
void CRR::setOType(OptionType o) { oType = o; };

void CRR::saveModelJson(const std::string& filepath) {
    auto greeks_values = std::make_shared<JsonObject>(JsonObject{
        {"delta", JsonValue(greeks.getDelta())}, 
        {"gamma", JsonValue(greeks.getGamma())},
        {"vega", JsonValue(greeks.getVega())},
        {"rho", JsonValue(greeks.getTheta())},
        {"theta", JsonValue(greeks.getTheta())}});

    auto outputs_values = std::make_shared<JsonObject>(JsonObject{
        {"option_price", JsonValue(result)},
        {"up_factor", JsonValue(u)},
        {"down_factor", JsonValue(d)},
        {"risk_neutral_probability", JsonValue(riskNeutral)},
        {"greeks", JsonValue(greeks_values)}});

    auto inputs_values = std::make_shared<JsonObject>(JsonObject{
        {"option_type", JsonValue((oType == CALL ? 0 : 1))},
        {"exercize_type", JsonValue((eType == EUROPEAN ? 0 : 1))},
        {"spot", JsonValue(S0)},  
        {"strike", JsonValue(K)},
        {"expiration", JsonValue(T)},
        {"risk_free_rate", JsonValue(r)},
        {"volatility", JsonValue(sigma)},
        {"steps", JsonValue(N)}});

    std::vector<std::pair<std::string, JsonValue>> values;
    values.emplace_back("inputs", JsonValue(inputs_values));
    values.emplace_back("outputs", JsonValue(outputs_values));

    JsonParser parser;
    parser.storeData(filepath, values);
}

void CRR::loadModelJson(const std::string& filepath, bool recompute) {
    JsonParser parser;
    JsonValue root = parser.parseJson(filepath);

    const JsonObject& rootObj = asObject(root);
    const JsonObject& in = asObject(field(rootObj, "inputs"));

    oType = (asNumber(field(in, "option_type")) == 0) ? CALL : PUT;
    eType = (asNumber(field(in, "exercize_type")) == 0) ? EUROPEAN : AMERICAN;
    S0 = asNumber(field(in, "spot"));
    K = asNumber(field(in, "strike"));
    T = asNumber(field(in, "expiration"));
    r = asNumber(field(in, "risk_free_rate"));
    sigma = asNumber(field(in, "volatility"));
    N = static_cast<int>(asNumber(field(in, "steps")));

    if (recompute) {
        const JsonObject& out = asObject(field(rootObj, "outputs"));
        const JsonObject& gr= asObject(field(out, "greeks"));
        result = asNumber(field(out, "option_price"));
        computed = true;
        u = asNumber(field(out, "up_factor"));
        d = asNumber(field(out, "down_factor"));
        riskNeutral = asNumber(field(out, "risk_neutral_probability"));

        greeks.setDelta(asNumber(field(gr, "delta")));
        greeks.setGamma(asNumber(field(gr, "gamma")));
        greeks.setVega (asNumber(field(gr, "vega")));
        greeks.setRho(asNumber(field(gr, "rho")));
        greeks.setTheta(asNumber(field(gr, "theta")));
    }
}