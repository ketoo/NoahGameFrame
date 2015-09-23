# ��ӭʹ��NFrame���ݷ������������

�ؼ��֣��ĵ������ܣ����ţ��̳�

**NFrame**һ��ʹ��C++���Կ����ġ�֧�ָ߲����������ܵĿ�ƽ̨���ݷ������������������
ּ�ڰ�����С��ҵ���Ϳ����ż������������Ŀ���ܡ��������ݿ����еķֲ����˼·�������ܲ��Ϊ������ģ�飬�ÿ�����Ա���д���һ���ܣ�����Ŷ�Ч�ʡ��ص������

* ͨ�õĳ������ϵͳ
* �������� (Property & record)
* �¼����� (Event)
* ����չ��App���������ģ�黯 (Plugin & Module)
* ����ӿڱ�� (IOD)
* �����ܡ��߲��� (���硢Actor���߼�)
* Component��� (�ű�ϵͳ)
* �ֲ�ʽ�������ܹ�
* ���ȶ��ԡ����ײ���֧����չ����ƽ̨
* ���ӻ����á����׹���
* ���׿ͻ���(Unity3D�ͻ���(�����)��Cocos2D�ͻ���(��δ��ʼ))
* ��ҵ���ƻ�����(�洢�������߼�ģ�顢�¼ܹ�)


**�ĵ�Ŀ¼**

[TOCM]

[TOC]
## ����
NFrame���ó�������ݿ���˼�롪���ֲ���ƣ��ֲ�ĳ�����ƴ����ĺô����Զ��׼��ģ����ڲ����ɢ����Ϲ�ϵ��ʹ�����ǿ���רע�ڱ������ƣ������ع������������ƣ�Ҳ���ص����Լ�����ƻ�Ӱ�������㣬�������������������档���ҷֲ����ʹ�ó���ṹ������������ά�������ʮ�����ף����Ĳ�ľ���ʵ�ִ��룬ֻҪ��ӿڱ����ȶ�����������Բ����޸ġ���ʹ��Ľӿڷ����仯��ҲֻӰ���ϲ���²㣬�޸Ĺ�����С���Ҵ�����Կ��ƣ������������ķ��ա�NFrameͬʱʹ���˽�Ӧ�ó�����Ƴ�����ܹ��������App���м���Ǹ��ֲ����������Ǹ��ֶ�Ӧ�ľ߻���ģ�鹦�ܡ�������Ƶ��ŵ��Ƕ�Ӧģ��ֻ�����Լ������񣬽�����ϣ�ͨ���ӿ�������ģ�齻������ģ��ķ��ս�����͡�

**App��Ƽܹ�ͼ**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_app_arch.png)

## ������ϸ����
### ͨ�õĳ������ϵͳ(LogicClass)
- �ḻ�Ļ�����������
- �������ԵĿ�������(XML���Զ�����������)
- �����ʼ���ݵĿ�������(XML����Ԥ��ֵ�������Ե�ֵ)
- �ɶ�̬��������(���������й����п��Գ����������)
- �����ڴ������������κ�ҵ����(XMLֱ������)
- ͨ�õ�����/��ȡ��Ϣ�ӿ�

### ��������
&#160; &#160; &#160; &#160;����ڴ�ͳ�ķ�����������NFrameʹ����һ��ȫ�µ����ݶ����ʹ�õķ��������ǳ�֮Ϊ����(Property)�ͱ�(Record)��
&#160; &#160; &#160; &#160;����(Property)��Ҫ�����洢�û��Ļ������ݣ����磺�������Ա����䡢�ȼ� �����ݣ���Ҫ����Ϊһ�����ƶ�Ӧһ�����ݡ�
&#160; &#160; &#160; &#160;��(Record)��Ҫ�����洢һЩ��¼�����磺�����б������б� �����ݣ���Ҫ����Ϊһ����¼������������ݡ�
&#160; &#160; &#160; &#160;NFrameʹ���˴���ģ��������Ӧ���е��������ݣ�������������ͳ�����������ݽṹ������ң��ӿڲ�ͳһ�������޷����ֵ����⡣

**һ��Property��Record�����ӣ�**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Property_Sample.png)
> NF������������(Excel�༭)

![NF Record Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Record_Sample.png)
> NF����������(Excel�༭)

### �¼�����
&#160; &#160; &#160; &#160;�¼����������Դ�봦�����Ĵ������̣�ּΪֻ�ṩ��ˮ��ʽ�Ĵ����߼�ģ�飬�����������������������ݡ�
�¼�����������Property������Record������Event������Heartbeat����
&#160; &#160; &#160; &#160;ͨ��Property Driver������ֻҪע������Թ۲��ߵ�Processer Function����õ�����עProperty�ı仯֪ͨ���Ա�������Ӧ���߼�����
&#160; &#160; &#160; &#160;ͨ��Record Driver������ֻҪע���Record�Ĺ۲��ߵ�Processer Function����õ�����עRecord�ı仯֪ͨ���Ա�������Ӧ���߼�����
&#160; &#160; &#160; &#160;ͨ��Event System������ֻҪע���Event�Ĺ۲��ߵ�Processer Function����õ�����ע���¼�֪ͨProcesser���Բ����µ��¼��������������Ա����������߼�ģ�鴦���߼���
&#160; &#160; &#160; &#160;ͨ��Heartbeat System������ֻҪע���ͬ�������Ĺ۲��ߵ�Processer Function���ᶨʱ�����߼����Ա���ʱ/��ʱ�����߼���

**NFrame�¼��������ͼ**
![NF Property Sample](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_Event_driver.png)

**NFrame�¼�����ʾ������**
Property����ʾ����
```cpp
m_pKernelModule->AddPropertyCallBack(self, "Level", this, &NFCPropertyModule::OnObjectLevelEvent);

m_pKernelModule->SetPropertyInt(self, "Level", 100);

int NFCPropertyModule::OnObjectLevelEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{
    // do something
    return 0;
}
```
Record��������ʾ����
```cpp
m_pKernelModule->AddRecordCallBack(self, "TaskList", this, &NFCHeroModule::OnTaskRecordEvent);

int NFCHeroModule::OnHeroRecordEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{
    NF_SHARED_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (nullptr == pObject)
    {
        return 1;
    }

    NF_SHARED_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (nullptr == pRecord)
    {
        return 1;
    }

    switch (nOpType)
    {
    case NFIRecord::Add:
        // TODO
        break;
    case NFIRecord::Del:
        // TODO
        break;
    case NFIRecord::UpData:
        // TODO
        break;
    default:
        break;
    }

    return 0;
}
```

Event��������ʾ��
```cpp
m_pEventProcessModule->AddEventCallBack(self, NF_EVENT_DO_SOMETHING, this, &NFCFightValueModule::OnDoSomethingEvent);

int NFCFightValueModule::OnRefreshFightValueEvent(const NFIDENTID& self, const int nEventID, const NFIDataList& var)
{
    // do something
	return 0;
}
```

HeartBeat��������ʾ����
```cpp
m_pKernelModule->AddHeartBeat(self, "OnHeartBeat", this, &HelloWorld3Module::OnHeartBeat, 5.0f, 1000);

int HelloWorld3Module::OnHeartBeat(const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    // do something
	return 0;
}
```

### ����ӿڱ��(IOD)
&#160; &#160; &#160; &#160;���ڴ����OOʽ������NFrame֧�ָ�����IO(�ӿ�)ʽ����������Ŀ������򵥴��⡣
ͨ��ģ�����������ӿ���ģ��Ĺ��ܻ�����ã�������������������ĵ���ϸ��ھۡ�

**NFrame����ӿڱ��ʾ������**
```cpp
class NFISceneProcessModule
    : public NFILogicModule
{

public:
    virtual bool IsCloneScene(const int nSceneID) = 0;
    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID) = 0;
};
```

### �����ܣ��߲���
&#160; &#160; &#160; &#160;NFrame��������ϵķֲ�����Ӷ�ʹ�üܹ��ϱ�������ܽϸߡ�ͬʱ������ͨ����ʹ���˾þ������LibEvent��Ϊ����ײ㣬ʹ��google ProtoBuf��ΪЭ�����л��⣬LibEvent�ĸ����ܼ���Protobuf�ĸ�ѹ���ʣ���ʵ���Թ���������8000�����û���Ƶ��Э��ͨѶ��
&#160; &#160; &#160; &#160;NFrame���߼�������ʹ����Actorģʽ��������Theron��ΪActor������⣬֧�ֲ�����̣��������CPU���ܡ� Theron��Ϊ֪����ҵ�������⣬Ӧ������๤ҵ������ϣ�����Matlabʹ����Theron����������ֱ�������4.5x��([ԭ������](http://www.theron-library.com/index.php?t=story&p=43))��
&#160; &#160; &#160; &#160;NFrameʹ��C++��Ϊ�����������ԣ����������������ԣ������ܺ�Ч���ϸ��ǿ���һ�������õ����ģʽ��Ӧ�����߼����Ӽ򵥡�

### ���ϵͳ(Component)
- �ṩ����Unity��ʽ��������ģʽ���Էḻ���������ڽű��༭�Լ�������Ϊ��չ
- ϵͳ�ڶ������ʱ�Զ�ʶ���Լ�ʵ����Component
- ͨ���¼�ϵͳ����Componentע���¼���Ϣ�����ڴ����߼�
- ������������⴦�������չ�������Ϊ
- ���ù�������ģ�飬�������
- ��ǰ�Ѿ�֧��lua��Ϊ�ű�Ƕ�뵽�����ʹ��(����̳�6)
- ���ڽ�֧��JavaScript��C#��python�Ƚű�����

### �ֲ�ʽ�������ܹ�
�������ǲο������ܹ��Լ����ڵ���Ŀ������ۣ�NFrame�����Ϊ�����Ͽ�������չ�ķֲ�ʽ�ܹ���ӵ�������������������޵�ϵͳ��ģ�͸��ؿ���չ�ԡ�

**��Ҫ���ԣ�**
1. ������չ
2. �߿ɿ���
3. ����ͨѶ
4. ҵ��ȥ���Ļ�
5. �Զ�ժ������ڵ�
6. �ڵ�ʵʱȨ�����������
7. ���ؾ���

�������ܹ�ͼ��
![NF Architecture](https://github.com/ketoo/NoahGameFrame/blob/develop/Introduce/img/NF_server_arch.png)

## �򵥽̳�
### �̳�1:���һ��ģ��
### �̳�2:��������
### �̳�3:�¼�ϵͳ
### �̳�4:�첽�¼�ϵͳ��actor
## ����