function [out,net]=forward_feed(net,data)
  
  nlayer=size(net.layer)(2);
  
  out=data;
  
  for i=1:nlayer
    layer=net.layer{i};
    
    net.layer{i}.in=[out 1];
    outweight=[out 1]*layer.weight;
    out=activation(layer.func,outweight);

    net.layer{i}.out=out;
    net.layer{i}.outweight=outweight;
    
  end
end