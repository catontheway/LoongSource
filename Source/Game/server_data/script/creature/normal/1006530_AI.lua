
-- �������ս������/ʾ���ű�
function c1006530_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 30099)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1006530, 2, "c1006530_OnEnterCombat")

--����ʱ���ڵ�ͼ����<1129, 5696, 975>ˢ�³�������̽î�
--����ʱ����
function c1006530_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--������������
	cre.MonsterSay(MapID, InstanceID, TargetID, 30100)
	map.MapCreateCreature(MapID, InstanceID, 3040522, 1129, 5696, 975)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

--ע��
aux.RegisterCreatureEvent(1006530, 4, "c1006530_OnDie")



