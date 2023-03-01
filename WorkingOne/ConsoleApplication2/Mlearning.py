def test(dataset):
   model = SimpleExpSmoothing(dataset, initialization_method="heuristic").fit(optimized=True)
   fit  = model.fittedvalues
   pred = model.forecast()
   print(fit)
   print(pred)
   return (fit, pred)
