//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from: NFMsgShare.proto
// Note: requires additional types generated from: NFDefine.proto
// Note: requires additional types generated from: NFMsgBase.proto
// Note: requires additional types generated from: NFMsgBaseEx.proto
namespace NFMsg
{
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqEnterGameServer")]
  public partial class ReqEnterGameServer : global::ProtoBuf.IExtensible
  {
    public ReqEnterGameServer() {}
    
    private int _server_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"server_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int server_id
    {
      get { return _server_id; }
      set { _server_id = value; }
    }
    private int _gate_id;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"gate_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int gate_id
    {
      get { return _gate_id; }
      set { _gate_id = value; }
    }
    private byte[] _name;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"name", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] name
    {
      get { return _name; }
      set { _name = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqLeaveGameServer")]
  public partial class ReqLeaveGameServer : global::ProtoBuf.IExtensible
  {
    public ReqLeaveGameServer() {}
    
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAckPlayerMove")]
  public partial class ReqAckPlayerMove : global::ProtoBuf.IExtensible
  {
    public ReqAckPlayerMove() {}
    
    private long _mover = default(long);
    [global::ProtoBuf.ProtoMember(1, IsRequired = false, Name=@"mover", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    [global::System.ComponentModel.DefaultValue(default(long))]
    public long mover
    {
      get { return _mover; }
      set { _mover = value; }
    }
    private int _moveType;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"moveType", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int moveType
    {
      get { return _moveType; }
      set { _moveType = value; }
    }
    private float _target_x;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"target_x", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float target_x
    {
      get { return _target_x; }
      set { _target_x = value; }
    }
    private float _target_y = default(float);
    [global::ProtoBuf.ProtoMember(4, IsRequired = false, Name=@"target_y", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    [global::System.ComponentModel.DefaultValue(default(float))]
    public float target_y
    {
      get { return _target_y; }
      set { _target_y = value; }
    }
    private float _target_z;
    [global::ProtoBuf.ProtoMember(5, IsRequired = true, Name=@"target_z", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float target_z
    {
      get { return _target_z; }
      set { _target_z = value; }
    }
    private float _source_x;
    [global::ProtoBuf.ProtoMember(6, IsRequired = true, Name=@"source_x", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float source_x
    {
      get { return _source_x; }
      set { _source_x = value; }
    }
    private float _source_y = default(float);
    [global::ProtoBuf.ProtoMember(7, IsRequired = false, Name=@"source_y", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    [global::System.ComponentModel.DefaultValue(default(float))]
    public float source_y
    {
      get { return _source_y; }
      set { _source_y = value; }
    }
    private float _source_z;
    [global::ProtoBuf.ProtoMember(8, IsRequired = true, Name=@"source_z", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float source_z
    {
      get { return _source_z; }
      set { _source_z = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAckPlayerChat")]
  public partial class ReqAckPlayerChat : global::ProtoBuf.IExtensible
  {
    public ReqAckPlayerChat() {}
    
    private long _chat_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"chat_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long chat_id
    {
      get { return _chat_id; }
      set { _chat_id = value; }
    }
    private NFMsg.ReqAckPlayerChat.EGameChatType _chat_type;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"chat_type", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public NFMsg.ReqAckPlayerChat.EGameChatType chat_type
    {
      get { return _chat_type; }
      set { _chat_type = value; }
    }
    private byte[] _chat_info;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"chat_info", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] chat_info
    {
      get { return _chat_info; }
      set { _chat_info = value; }
    }
    [global::ProtoBuf.ProtoContract(Name=@"EGameChatType")]
    public enum EGameChatType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCT_NARMAL", Value=0)]
      EGCT_NARMAL = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCT_PRIVATE", Value=1)]
      EGCT_PRIVATE = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCT_TEAM", Value=2)]
      EGCT_TEAM = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCT_WORLD", Value=3)]
      EGCT_WORLD = 3
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAckUseSkill")]
  public partial class ReqAckUseSkill : global::ProtoBuf.IExtensible
  {
    public ReqAckUseSkill() {}
    
    private byte[] _skill_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"skill_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] skill_id
    {
      get { return _skill_id; }
      set { _skill_id = value; }
    }
    private readonly global::System.Collections.Generic.List<long> _effect_ident = new global::System.Collections.Generic.List<long>();
    [global::ProtoBuf.ProtoMember(2, Name=@"effect_ident", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<long> effect_ident
    {
      get { return _effect_ident; }
    }
  
    private readonly global::System.Collections.Generic.List<int> _effect_value = new global::System.Collections.Generic.List<int>();
    [global::ProtoBuf.ProtoMember(3, Name=@"effect_value", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<int> effect_value
    {
      get { return _effect_value; }
    }
  
    private readonly global::System.Collections.Generic.List<int> _effect_rlt = new global::System.Collections.Generic.List<int>();
    [global::ProtoBuf.ProtoMember(4, Name=@"effect_rlt", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<int> effect_rlt
    {
      get { return _effect_rlt; }
    }
  
    private byte[] _key_value = null;
    [global::ProtoBuf.ProtoMember(5, IsRequired = false, Name=@"key_value", DataFormat = global::ProtoBuf.DataFormat.Default)]
    [global::System.ComponentModel.DefaultValue(null)]
    public byte[] key_value
    {
      get { return _key_value; }
      set { _key_value = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAckUseItem")]
  public partial class ReqAckUseItem : global::ProtoBuf.IExtensible
  {
    public ReqAckUseItem() {}
    
    private int _row;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"row", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int row
    {
      get { return _row; }
      set { _row = value; }
    }
    private long _item_guid;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"item_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long item_guid
    {
      get { return _item_guid; }
      set { _item_guid = value; }
    }
    private readonly global::System.Collections.Generic.List<long> _effect_ident = new global::System.Collections.Generic.List<long>();
    [global::ProtoBuf.ProtoMember(3, Name=@"effect_ident", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<long> effect_ident
    {
      get { return _effect_ident; }
    }
  
    private readonly global::System.Collections.Generic.List<int> _effect_value = new global::System.Collections.Generic.List<int>();
    [global::ProtoBuf.ProtoMember(4, Name=@"effect_value", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<int> effect_value
    {
      get { return _effect_value; }
    }
  
    private readonly global::System.Collections.Generic.List<int> _effect_rlt = new global::System.Collections.Generic.List<int>();
    [global::ProtoBuf.ProtoMember(5, Name=@"effect_rlt", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<int> effect_rlt
    {
      get { return _effect_rlt; }
    }
  
    private byte[] _key_value = null;
    [global::ProtoBuf.ProtoMember(6, IsRequired = false, Name=@"key_value", DataFormat = global::ProtoBuf.DataFormat.Default)]
    [global::System.ComponentModel.DefaultValue(null)]
    public byte[] key_value
    {
      get { return _key_value; }
      set { _key_value = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAckSwapScene")]
  public partial class ReqAckSwapScene : global::ProtoBuf.IExtensible
  {
    public ReqAckSwapScene() {}
    
    private NFMsg.ReqAckSwapScene.EGameSwapType _transfer_type;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"transfer_type", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public NFMsg.ReqAckSwapScene.EGameSwapType transfer_type
    {
      get { return _transfer_type; }
      set { _transfer_type = value; }
    }
    private int _scene_id;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"scene_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int scene_id
    {
      get { return _scene_id; }
      set { _scene_id = value; }
    }
    private int _line_id;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"line_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int line_id
    {
      get { return _line_id; }
      set { _line_id = value; }
    }
    private float _x = default(float);
    [global::ProtoBuf.ProtoMember(4, IsRequired = false, Name=@"x", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    [global::System.ComponentModel.DefaultValue(default(float))]
    public float x
    {
      get { return _x; }
      set { _x = value; }
    }
    private float _y = default(float);
    [global::ProtoBuf.ProtoMember(5, IsRequired = false, Name=@"y", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    [global::System.ComponentModel.DefaultValue(default(float))]
    public float y
    {
      get { return _y; }
      set { _y = value; }
    }
    private float _z = default(float);
    [global::ProtoBuf.ProtoMember(6, IsRequired = false, Name=@"z", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    [global::System.ComponentModel.DefaultValue(default(float))]
    public float z
    {
      get { return _z; }
      set { _z = value; }
    }
    [global::ProtoBuf.ProtoContract(Name=@"EGameSwapType")]
    public enum EGameSwapType
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGST_NARMAL", Value=0)]
      EGST_NARMAL = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGST_CLONE", Value=1)]
      EGST_CLONE = 1,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGST_ARENA", Value=2)]
      EGST_ARENA = 2,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGST_MIRROR", Value=3)]
      EGST_MIRROR = 3
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqSellItem")]
  public partial class ReqSellItem : global::ProtoBuf.IExtensible
  {
    public ReqSellItem() {}
    
    private long _item_guid;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"item_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long item_guid
    {
      get { return _item_guid; }
      set { _item_guid = value; }
    }
    private long _item_count;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"item_count", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long item_count
    {
      get { return _item_count; }
      set { _item_count = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqSplitItem")]
  public partial class ReqSplitItem : global::ProtoBuf.IExtensible
  {
    public ReqSplitItem() {}
    
    private long _item_guid;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"item_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long item_guid
    {
      get { return _item_guid; }
      set { _item_guid = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqProduceItem")]
  public partial class ReqProduceItem : global::ProtoBuf.IExtensible
  {
    public ReqProduceItem() {}
    
    private byte[] _item_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"item_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] item_id
    {
      get { return _item_id; }
      set { _item_id = value; }
    }
    private int _count = default(int);
    [global::ProtoBuf.ProtoMember(2, IsRequired = false, Name=@"count", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    [global::System.ComponentModel.DefaultValue(default(int))]
    public int count
    {
      get { return _count; }
      set { _count = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqPickDropItem")]
  public partial class ReqPickDropItem : global::ProtoBuf.IExtensible
  {
    public ReqPickDropItem() {}
    
    private long _npc_guid;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"npc_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long npc_guid
    {
      get { return _npc_guid; }
      set { _npc_guid = value; }
    }
    private long _item_guid;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"item_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long item_guid
    {
      get { return _item_guid; }
      set { _item_guid = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqAcceptTask")]
  public partial class ReqAcceptTask : global::ProtoBuf.IExtensible
  {
    public ReqAcceptTask() {}
    
    private byte[] _task_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"task_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] task_id
    {
      get { return _task_id; }
      set { _task_id = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqPullDownTask")]
  public partial class ReqPullDownTask : global::ProtoBuf.IExtensible
  {
    public ReqPullDownTask() {}
    
    private byte[] _task_id;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"task_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] task_id
    {
      get { return _task_id; }
      set { _task_id = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"CloneCustomItem")]
  public partial class CloneCustomItem : global::ProtoBuf.IExtensible
  {
    public CloneCustomItem() {}
    
    private long _object_guid;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"object_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long object_guid
    {
      get { return _object_guid; }
      set { _object_guid = value; }
    }
    private readonly global::System.Collections.Generic.List<byte[]> _item_id = new global::System.Collections.Generic.List<byte[]>();
    [global::ProtoBuf.ProtoMember(2, Name=@"item_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<byte[]> item_id
    {
      get { return _item_id; }
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"ReqPullDownCustoms")]
  public partial class ReqPullDownCustoms : global::ProtoBuf.IExtensible
  {
    public ReqPullDownCustoms() {}
    
    private NFMsg.ReqPullDownCustoms.EGameCloneResult _result_value;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"result_value", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public NFMsg.ReqPullDownCustoms.EGameCloneResult result_value
    {
      get { return _result_value; }
      set { _result_value = value; }
    }
    private readonly global::System.Collections.Generic.List<NFMsg.CloneCustomItem> _item_list = new global::System.Collections.Generic.List<NFMsg.CloneCustomItem>();
    [global::ProtoBuf.ProtoMember(2, Name=@"item_list", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<NFMsg.CloneCustomItem> item_list
    {
      get { return _item_list; }
    }
  
    [global::ProtoBuf.ProtoContract(Name=@"EGameCloneResult")]
    public enum EGameCloneResult
    {
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCR_SUCCESS", Value=0)]
      EGCR_SUCCESS = 0,
            
      [global::ProtoBuf.ProtoEnum(Name=@"EGCR_FAILED", Value=1)]
      EGCR_FAILED = 1
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"PlayerEntryInfo")]
  public partial class PlayerEntryInfo : global::ProtoBuf.IExtensible
  {
    public PlayerEntryInfo() {}
    
    private long _object_guid;
    [global::ProtoBuf.ProtoMember(1, IsRequired = true, Name=@"object_guid", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public long object_guid
    {
      get { return _object_guid; }
      set { _object_guid = value; }
    }
    private int _char_type;
    [global::ProtoBuf.ProtoMember(2, IsRequired = true, Name=@"char_type", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int char_type
    {
      get { return _char_type; }
      set { _char_type = value; }
    }
    private float _x;
    [global::ProtoBuf.ProtoMember(3, IsRequired = true, Name=@"x", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float x
    {
      get { return _x; }
      set { _x = value; }
    }
    private float _y;
    [global::ProtoBuf.ProtoMember(4, IsRequired = true, Name=@"y", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float y
    {
      get { return _y; }
      set { _y = value; }
    }
    private float _z;
    [global::ProtoBuf.ProtoMember(5, IsRequired = true, Name=@"z", DataFormat = global::ProtoBuf.DataFormat.FixedSize)]
    public float z
    {
      get { return _z; }
      set { _z = value; }
    }
    private int _career_type;
    [global::ProtoBuf.ProtoMember(6, IsRequired = true, Name=@"career_type", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int career_type
    {
      get { return _career_type; }
      set { _career_type = value; }
    }
    private int _player_state;
    [global::ProtoBuf.ProtoMember(7, IsRequired = true, Name=@"player_state", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public int player_state
    {
      get { return _player_state; }
      set { _player_state = value; }
    }
    private byte[] _config_id;
    [global::ProtoBuf.ProtoMember(8, IsRequired = true, Name=@"config_id", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public byte[] config_id
    {
      get { return _config_id; }
      set { _config_id = value; }
    }
    private byte[] _class_name = null;
    [global::ProtoBuf.ProtoMember(9, IsRequired = false, Name=@"class_name", DataFormat = global::ProtoBuf.DataFormat.Default)]
    [global::System.ComponentModel.DefaultValue(null)]
    public byte[] class_name
    {
      get { return _class_name; }
      set { _class_name = value; }
    }
    private int _scene_id = default(int);
    [global::ProtoBuf.ProtoMember(10, IsRequired = false, Name=@"scene_id", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    [global::System.ComponentModel.DefaultValue(default(int))]
    public int scene_id
    {
      get { return _scene_id; }
      set { _scene_id = value; }
    }
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"AckPlayerEntryList")]
  public partial class AckPlayerEntryList : global::ProtoBuf.IExtensible
  {
    public AckPlayerEntryList() {}
    
    private readonly global::System.Collections.Generic.List<NFMsg.PlayerEntryInfo> _object_list = new global::System.Collections.Generic.List<NFMsg.PlayerEntryInfo>();
    [global::ProtoBuf.ProtoMember(1, Name=@"object_list", DataFormat = global::ProtoBuf.DataFormat.Default)]
    public global::System.Collections.Generic.List<NFMsg.PlayerEntryInfo> object_list
    {
      get { return _object_list; }
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
  [global::System.Serializable, global::ProtoBuf.ProtoContract(Name=@"AckPlayerLeaveList")]
  public partial class AckPlayerLeaveList : global::ProtoBuf.IExtensible
  {
    public AckPlayerLeaveList() {}
    
    private readonly global::System.Collections.Generic.List<long> _object_list = new global::System.Collections.Generic.List<long>();
    [global::ProtoBuf.ProtoMember(1, Name=@"object_list", DataFormat = global::ProtoBuf.DataFormat.TwosComplement)]
    public global::System.Collections.Generic.List<long> object_list
    {
      get { return _object_list; }
    }
  
    private global::ProtoBuf.IExtension extensionObject;
    global::ProtoBuf.IExtension global::ProtoBuf.IExtensible.GetExtensionObject(bool createIfMissing)
      { return global::ProtoBuf.Extensible.GetExtensionObject(ref extensionObject, createIfMissing); }
  }
  
}