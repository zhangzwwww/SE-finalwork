import torchvision as tv
import torchvision.transforms as transforms
import torch
from PIL import Image

from mlModel import Network

def predict():
    model = Network()
    model.load_state_dict(torch.load('./model_param/model_states.pth'))

    img = Image.open('./1.jpg')
    transform = transforms.Compose([
        transforms.RandomResizedCrop(224),
        transforms.ToTensor(), # Transform from [0,255] uint8 to [0,1] float
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]) # Normalize to zero mean and unit variance
    ])
    img = transform(img)
    img = img.unsqueeze(0)

    with torch.no_grad():
        model.eval()
        py = model(img)
    res = torch.max(py, 1)[1].data.numpy()
    print(res[0])


predict()
