--1005419	ħԳ��(����ħ��) /ʾ���ű�
--����ս�����ٻ� {1005411 ��������[��ǰ]} {1005412 ��������[��ǰ]}
function c1005419_On_Enter_Combat(MapID, InstanceID, CreatureID)
	-- �������ս������
	cre.MonsterSay(MapID, InstanceID, CreatureID, 20059)

	--local x, y ,z = unit.GetPosition(MapID, InstanceID, CreatureID)
	--map.MapCreateCreature(MapID, InstanceID, 1005411, x-5, y, z)
	--map.MapCreateCreature(MapID, InstanceID, 1005412, x+5, y, z)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end
aux.RegisterCreatureEvent(1005419,2,"c1005419_On_Enter_Combat")

--������������
function c1005419_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 20061)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")


	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 8)+2
	map.SetMapScriptData(MapID, InstanceID, 1, 8, Boss_num)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1756)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

aux.RegisterCreatureEvent(1005419, 4, "c1005419_OnDie")
